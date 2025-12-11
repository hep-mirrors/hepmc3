#ifdef HEPMC3_EDM4HEP
#include "WriterEDM4HEP.h"

// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file WriterEDM4HEP.cc
/// @brief Implementation of \b class WriterEDM4HEP
///
#include <cstring>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Version.h"

#include "edm4hep/EDM4hepVersion.h"
#include "edm4hep/Constants.h"
#include "edm4hep/GeneratorEventParametersCollection.h"
#include "edm4hep/GeneratorToolInfo.h"

#include "edm4hep/EventHeaderCollection.h"
#include "edm4hep/MCParticleCollection.h"

#include "HepMC3/Attribute.h"
#include "HepPDT/ParticleID.hh"

#include "podio/Frame.h"

namespace HepMC3 {

// for the plugin
HEPMC3_DECLARE_WRITER_FILE(WriterEDM4HEP)

WriterEDM4HEP::WriterEDM4HEP(const std::string& filename, std::shared_ptr<GenRunInfo> run)
    : m_particle_counter(0), m_edm4hepWriterClosed(false),
      m_edm4hepWriter(filename) {
    set_run_info(run);
    if (!run_info())
        set_run_info(std::make_shared<GenRunInfo>());
    if (m_edm4hepWriterClosed) {
        HEPMC3_ERROR_LEVEL(100, "WriterEDM4HEP: could not open output file: " << filename)
    }
}

WriterEDM4HEP::~WriterEDM4HEP() {
    if (!m_edm4hepWriterClosed)
        close();
}

void WriterEDM4HEP::write_event(const GenEvent& evt) {

    // make sure run_info is up to date
    if (!run_info()) {
        set_run_info(evt.run_info());
        write_run_info();
    }
    if (evt.run_info() && run_info() != evt.run_info()) {
        set_run_info(evt.run_info());
        write_run_info();
    }
    // now deal with the event
    auto eventFrame = podio::Frame();

    // here is the collection
    edm4hep::MCParticleCollection particleCollection;

    std::map<unsigned int, edm4hep::MutableMCParticle> mapIDPart;
    for (const auto& hepmcParticle : evt.particles()) {      
        // if it's a new particle, insert into the map
        if (mapIDPart.find(hepmcParticle->id()) == mapIDPart.end()) {
            edm4hep::MutableMCParticle edm_particle = transformParticle(hepmcParticle);
            mapIDPart.insert({hepmcParticle->id(), edm_particle});
        }
        // mother/daughter links
        auto prodvertex = hepmcParticle->production_vertex();
        if (nullptr != prodvertex) {
            for (auto particle_mother : prodvertex->particles_in()) {
                if (mapIDPart.find(particle_mother->id()) == mapIDPart.end()) {
                    edm4hep::MutableMCParticle edm_particle = transformParticle(particle_mother);
                    mapIDPart.insert({particle_mother->id(), edm_particle});
                }
                mapIDPart[hepmcParticle->id()].addToParents(mapIDPart[particle_mother->id()]);
            }
        }
        auto endvertex = hepmcParticle->end_vertex();
        if (nullptr != endvertex) {
            for (auto particle_daughter : endvertex->particles_out()) {
                if (mapIDPart.find(particle_daughter->id()) == mapIDPart.end()) {
                    auto edm_particle = transformParticle(particle_daughter);
                    mapIDPart.insert({particle_daughter->id(), edm_particle});
                }
                mapIDPart[hepmcParticle->id()].addToDaughters(mapIDPart[particle_daughter->id()]);
            }
        }
    }

    for (auto particle_pair : mapIDPart) {
        particleCollection.push_back(particle_pair.second);
    }

    // now we need the event header
    edm4hep::EventHeaderCollection evtHeaderCollection;
    edm4hep::MutableEventHeader evtHeader;

    // add eventNumber
    evtHeader.setEventNumber(evt.event_number());

    // add the weights
    for (auto weight : evt.weights()) {
        evtHeader.addToWeights(weight);
    }
    // the eventheader weight has to be set separately
    if (evt.weights().size() > 0) {
        evtHeader.setWeight(evt.weights()[0]);
    }

    // push to collection
    evtHeaderCollection.push_back(evtHeader);

    // write the EventHeader collection to the frame
    eventFrame.put(std::move(evtHeaderCollection), edm4hep::labels::EventHeader);

    // first the GeneratorEventParameters
    edm4hep::GeneratorEventParametersCollection generatorParametersCollection;
    edm4hep::MutableGeneratorEventParameters generatorParameters;

    // add the cross sections and its errors as parameter vector to the Frame
    if (evt.cross_section()) {
        for (auto xsec : evt.cross_section()->xsecs()) {
            generatorParameters.addToCrossSections(xsec);
        }
        for (auto xsecErr : evt.cross_section()->xsec_errs()) {
            generatorParameters.addToCrossSectionErrors(xsecErr);
        }
    }

    // add SQRTS
    double sqrts = 0.;
    if (evt.beams().size() == 2) {
        ConstGenParticlePtr beam1 = evt.beams()[0];
        ConstGenParticlePtr beam2 = evt.beams()[1];
        sqrts = (beam1->momentum() + beam2->momentum()).m();
    }
    // special treatement for MADGRAPH: overwrite beams to get the SQRTS correctly
    if (run_info()->tools().size() > 0) {
        if (run_info()->tools()[0].name.find("MadGraph") != std::string::npos) {
            sqrts = retrieveDoubleAttribute(evt, "EBMUP1") + retrieveDoubleAttribute(evt, "EBMUP2");
        }
    }
    // now we write to the structure:
    generatorParameters.setSqrts(sqrts);

    // signal vertex ID
    bool convertOK =
        writeSignalVertex(evt, retrieveIntAttribute(evt, "signal_vertex_id"), mapIDPart, generatorParameters);
    // inconsistent use of attributes, fallback for SHERPA
    if (!convertOK) {
        writeSignalVertex(evt, retrieveIntAttribute(evt, "signal_process_vertex"), mapIDPart, generatorParameters);
    }

    // retrieve the pdf information
    HepMC3::ConstGenPdfInfoPtr pdfinfo = evt.pdf_info();
    if ( pdfinfo ) {
#if edm4hep_VERSION >= EDM4HEP_VERSION(0,99,4)
        generatorParameters.setPartonIds({pdfinfo->parton_id[0], pdfinfo->parton_id[1]});
#else
        /* Not implemented */
#endif
    }

    // add the object to the collection:
    generatorParametersCollection.push_back(generatorParameters);
    // write the GeneratorEventParameters collection to the frame
    eventFrame.put(std::move(generatorParametersCollection), edm4hep::labels::GeneratorEventParameters);

    // LAST ITEM: write the collection of MCParticles to the frame mv empties the collection which we need for
    // processing!!
    eventFrame.put(std::move(particleCollection), edm4hep::labels::MCParticles);

    // write the frame to the Writer:
    m_edm4hepWriter.writeFrame(eventFrame, podio::Category::Event);
}
bool WriterEDM4HEP::writeSignalVertex(const GenEvent& evt, int hepmcVertexID,
                                      std::map<unsigned int, edm4hep::MutableMCParticle>& mapHEPMC2EDM4HEP,
                                      edm4hep::MutableGeneratorEventParameters& generatorParameters) {

    bool convertOK = false;
    // retrieve the vertices
    for (auto vtx : evt.vertices()) {
        // identify the signalvertex via its hepmcID
        if (vtx->id() == hepmcVertexID) {
            convertOK = true;
            // now insert all incoming particles to the generatorParameters
            for (auto part : vtx->particles_in()) {
                unsigned int hepmcParticleID = part->id();
                if (mapHEPMC2EDM4HEP.find(hepmcParticleID) != mapHEPMC2EDM4HEP.end()) {
#if EDM4HEP_BUILD_VERSION >= EDM4HEP_VERSION(0,99,4)
                    generatorParameters.addToSignalVertexParticles(mapHEPMC2EDM4HEP[hepmcParticleID]);
#else
                    generatorParameters.addToSignalVertex(mapHEPMC2EDM4HEP[hepmcParticleID]);
#endif
                }
            }
        }
    }

    return convertOK;
}

double WriterEDM4HEP::retrieveDoubleAttribute(const GenEvent& evt, std::string name) {

    shared_ptr<HepMC3::DoubleAttribute> hepmcPtr = evt.attribute<HepMC3::DoubleAttribute>(name);
    double result = hepmcPtr ? (hepmcPtr->value()) : 0.0;

    return result;
}

double WriterEDM4HEP::retrieveIntAttribute(const GenEvent& evt, std::string name) {

    shared_ptr<HepMC3::IntAttribute> hepmcPtr = evt.attribute<HepMC3::IntAttribute>(name);
    int result = hepmcPtr ? (hepmcPtr->value()) : 0.0;

    return result;
}

void WriterEDM4HEP::write_run_info() {

    // create the frame
    auto runFrame = podio::Frame();

    // start with the generator information
    const std::vector<GenRunInfo::ToolInfo> listOfTools = run_info()->tools();
    if (listOfTools.size() == 0) {
        std::cout << "WARNING: no tools found, hepmc run_info incomplete" << std::endl;
    }

    // the EDM4HEP structure
    std::vector<edm4hep::GeneratorToolInfo> toolInfosVectEDM4HEP;
    for (auto hepmcTool : listOfTools) {
        edm4hep::GeneratorToolInfo edm4hepTool;
        edm4hepTool.name = hepmcTool.name;
        edm4hepTool.version = hepmcTool.version;
        edm4hepTool.description = hepmcTool.description;
        toolInfosVectEDM4HEP.push_back(edm4hepTool);
    }
    edm4hep::utils::putGenToolInfos(runFrame, toolInfosVectEDM4HEP);

    // weight names
    std::vector<std::string> weights = run_info()->weight_names();

    // add the weights as parameters to the frame
    runFrame.putParameter(edm4hep::labels::GeneratorWeightNames, weights);

    // write the frame to the Writer:
    m_edm4hepWriter.writeFrame(runFrame, podio::Category::Run);
}

edm4hep::MutableMCParticle WriterEDM4HEP::transformParticle(const ConstGenParticlePtr& hepmcParticle) {
    edm4hep::MutableMCParticle edm_particle;
    edm_particle.setPDG(hepmcParticle->pdg_id());
    edm_particle.setGeneratorStatus(hepmcParticle->status());
    // look up charge from pdg_id
    HepPDT::ParticleID particleID(hepmcParticle->pdg_id());
    edm_particle.setCharge(particleID.charge());

    // convert momentum
    auto p = hepmcParticle->momentum();
    edm_particle.setMomentum(edm4hep::Vector3d(p.px(), p.py(), p.pz()));

    // set the mass (energy is deduced in EDM4HEP
    edm_particle.setMass(p.m());

    // TODO: HELICITY, depends on HepMC

    // convert production vertex info and time info:
    auto prodVtx = hepmcParticle->production_vertex();
    if (prodVtx != nullptr) {
        auto& pos = prodVtx->position();
        edm_particle.setVertex(edm4hep::Vector3d(pos.x(), pos.y(), pos.z()));
        edm_particle.setTime(pos.t());
    }

    // convert the decay vertex (if present)
    auto endpointVtx = hepmcParticle->end_vertex();
    if (endpointVtx != nullptr) {
        auto& pos = endpointVtx->position();
        edm_particle.setEndpoint(edm4hep::Vector3d(pos.x(), pos.y(), pos.z()));
    }

    return edm_particle;
}

bool WriterEDM4HEP::failed() { return false; }
void WriterEDM4HEP::close() {
    m_edm4hepWriter.finish();
    m_edm4hepWriterClosed = true;
}
} // namespace HepMC3
#endif
