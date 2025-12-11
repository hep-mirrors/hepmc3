#ifndef HEPMC3_WRITEREDM4HEP_H
#define HEPMC3_WRITEREDM4HEP_H
// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file  WriterEDM4HEP.h
/// @brief Definition of class \b WriterEDM4HEP
///
/// @class HepMC3::WriterEDM4HEP
/// @brief GenEvent I/O serialization for structured text files
///
/// @ingroup IO
///
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenRunInfo.h"
#include "HepMC3/Writer.h"
#include <fstream>
#include <string>

#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/MutableGeneratorEventParameters.h"
#include "podio/ROOTWriter.h"

namespace HepMC3 {

class WriterEDM4HEP : public Writer {
public:
    /// @brief Constructor
    /// @warning If file already exists, it will be cleared before writing
    WriterEDM4HEP(const std::string& filename, std::shared_ptr<GenRunInfo> run = std::shared_ptr<GenRunInfo>());

    /// @brief Constructor from ostream
    //  WriterEDM4HEP(std::ostream& stream, std::shared_ptr<GenRunInfo> run = std::shared_ptr<GenRunInfo>());
    /// @brief Constructor from temp ostream
    //  WriterEDM4HEP(std::shared_ptr<std::ostream> s_stream,
    //                std::shared_ptr<GenRunInfo> run = std::shared_ptr<GenRunInfo>());

    /// @brief Destructor
    ~WriterEDM4HEP();

    /// @brief Write event to file
    ///
    /// @param[in] evt Event to be serialized
    void write_event(const GenEvent& evt) override;

    /// @brief Write the GenRunInfo object to file.
    void write_run_info();

    bool failed() override;

    /// @brief Close file stream
    void close() override;

private:
    /// @brief Write particle
    ///
    /// Helper routine for writing single particle to file
    edm4hep::MutableMCParticle transformParticle(const ConstGenParticlePtr& p);

    /// Helper routine to translate the HepMC Vertex ID
    bool writeSignalVertex(const GenEvent& evt, int hepmcVertex,
                           std::map<unsigned int, edm4hep::MutableMCParticle>& mapHEPMC2PODIO,
                           edm4hep::MutableGeneratorEventParameters& genParams);

    /// retrieve the attribute of type double
    double retrieveDoubleAttribute(const GenEvent& evt, std::string name);

    /// retrieve the attribute of type int
    double retrieveIntAttribute(const GenEvent& evt, std::string name);

    /// @}

private:
    unsigned long m_particle_counter;              //!< Used to set bar codes

    bool m_edm4hepWriterClosed;
    podio::ROOTWriter m_edm4hepWriter;
};

} // namespace HepMC3

#endif
