// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderAsciiHepMC2.cc
 *  @brief Implementation of \b class ReaderAsciiHepMC2
 *
 */
#include <array>
#include <cstdlib>
#include <cstring>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenHeavyIon.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenPdfInfo.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/Setup.h"

namespace HepMC3 {

ReaderAsciiHepMC2::ReaderAsciiHepMC2(const std::string& filename):
    m_file(filename), m_stream(nullptr), m_isstream(false) {
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: could not open input file: " << filename )
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderAsciiHepMC2::ReaderAsciiHepMC2(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderAsciiHepMC2::ReaderAsciiHepMC2(std::shared_ptr<std::istream> s_stream)
    : m_shared_stream(s_stream), m_stream(s_stream.get()), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}


ReaderAsciiHepMC2::~ReaderAsciiHepMC2() {  if (!m_isstream) close(); }

bool ReaderAsciiHepMC2::skip(const int n)
{
    std::array<char, 262144> buf;
    int nn = n;
    while (!failed()) {
        char peek(0);
        if ( (!m_file.is_open()) && (!m_isstream) ) return false;
        m_isstream ? peek = m_stream->peek() : peek = m_file.peek();
        if ( peek =='E' ) nn--;
        if (nn < 0) { return true; }
        m_isstream ? m_stream->getline(buf.data(), buf.size()) : m_file.getline(buf.data(), buf.size());
    }
    return true;
}

bool ReaderAsciiHepMC2::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;

    char               peek = 0;
    std::array<char, 262144> buf;
    bool          parsed_event_header            = false;
    bool          is_parsing_successful          = true;
    int           parsing_result                 = 0;
    unsigned int  vertices_count                 = 0;
    unsigned int  current_vertex_particles_count = 0;
    unsigned int  current_vertex_particles_parsed = 0;

    evt.clear();
    evt.set_run_info(run_info());

    m_forward_mothers.clear();
    m_vertices.clear();
    m_particles.clear();
    m_last_vertex = nullptr;
    //
    // Parse event, vertex and particle information
    //
    while (!failed()) {
        m_isstream ? m_stream->getline(buf.data(), buf.size()) : m_file.getline(buf.data(), buf.size());
        if ( strlen(buf.data()) == 0 ) continue;
        // Check for IO_GenEvent header/footer
        if ( strncmp(buf.data(), "HepMC", 5) == 0 ) {
            if ( strncmp(buf.data(), "HepMC::Version", 14) != 0 && strncmp(buf.data(), "HepMC::IO_GenEvent", 18) != 0 )
            {
                HEPMC3_WARNING("ReaderAsciiHepMC2: found unsupported expression in header. Will close the input.")
                std::cout <<buf.data() << std::endl;
                m_isstream ? m_stream->clear(std::ios::eofbit) : m_file.clear(std::ios::eofbit);
            }
            if (parsed_event_header) {
                is_parsing_successful = true;
                break;
            }
            continue;
        }
        switch (buf[0]) {
        case 'E':
            parsing_result = parse_event_information(evt, buf.data());
            if (parsing_result < 0) {
                is_parsing_successful = false;
                HEPMC3_ERROR("ReaderAsciiHepMC2: HEPMC3_ERROR parsing event information")
            }
            else {
                vertices_count = parsing_result;
                // Here we make a trick: reserve for this event the
                // vertices_count*3 or the number of particles in the
                // prev. event.
                evt.reserve(vertices_count, m_particles.size());
                is_parsing_successful = true;
            }
            parsed_event_header = true;
            break;
        case 'V':
            // If starting new vertex: verify if previous was fully parsed

            /** @bug HepMC2 files produced with Pythia8 are known to have wrong
                     information about number of particles in vertex. Hence '<' sign */
            if (current_vertex_particles_parsed < current_vertex_particles_count) {
                is_parsing_successful = false;
                break;
            }
            current_vertex_particles_parsed = 0;

            parsing_result = parse_vertex_information(evt,buf.data());

            if (parsing_result < 0) {
                is_parsing_successful = false;
                HEPMC3_ERROR("ReaderAsciiHepMC2: HEPMC3_ERROR parsing vertex information")
            }
            else {
                current_vertex_particles_count = parsing_result;
                is_parsing_successful = true;
            }
            break;
        case 'P':

            parsing_result   = parse_particle_information(evt,buf.data());

            if (parsing_result < 0) {
                is_parsing_successful = false;
                HEPMC3_ERROR("ReaderAsciiHepMC2: HEPMC3_ERROR parsing particle information")
            }
            else {
                ++current_vertex_particles_parsed;
                is_parsing_successful = true;
            }
            break;
        case 'U':
            is_parsing_successful = parse_units(evt, buf.data());
            break;
        case 'F':
            is_parsing_successful = parse_pdf_info(evt, buf.data());
            break;
        case 'H':
            is_parsing_successful = parse_heavy_ion(evt, buf.data());
            break;
        case 'N':
            is_parsing_successful = parse_weight_names(buf.data());
            break;
        case 'C':
            is_parsing_successful = parse_xs_info(evt, buf.data());
            break;
        default:
            HEPMC3_WARNING("ReaderAsciiHepMC2: skipping unrecognised prefix: " << buf[0])
            is_parsing_successful = true;
            break;
        }

        if ( !is_parsing_successful ) break;

        // Check for next event
        m_isstream ? peek = m_stream->peek() : peek = m_file.peek();
        if ( parsed_event_header && peek == 'E' ) break;
    }

    // Check if all particles in last vertex were parsed
    /** @bug HepMC2 files produced with Pythia8 are known to have wrong
             information about number of particles in vertex. Hence '<' sign */
    if (is_parsing_successful && current_vertex_particles_parsed < current_vertex_particles_count) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: not all particles parsed")
        is_parsing_successful = false;
    }
    // Check if all vertices were parsed
    else if (is_parsing_successful && m_vertices.size() != vertices_count) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: not all vertices parsed")
        is_parsing_successful = false;
    }

    if ( !is_parsing_successful ) {
        HEPMC3_ERROR("ReaderAsciiHepMC2: event parsing failed. Returning empty event")
        HEPMC3_DEBUG(1, "Parsing failed at line:" << std::endl << buf.data())
        evt.clear();
        m_isstream ? m_stream->clear(std::ios::badbit) : m_file.clear(std::ios::badbit);
        return false;
    }
    if (run_info() && run_info()->weight_names().empty() ) {
        run_info()->set_weight_names(std::vector<std::string> {"Default"});
    }
    if (evt.weights().empty()) {
        HEPMC3_WARNING("ReaderAsciiHepMC2: weights are empty, an event weight 1.0 will be added.")
        evt.weights().push_back(1.0);
    }

    for (const auto& p : m_forward_mothers )
    {
        auto iter = m_vertices.find(p.second);
        if (iter != m_vertices.end())
            iter->second->add_particle_in(p.first);
        // GenVertexPtr v = evt.vertex(p.second);
        // if (v) v->add_particle_out(p.first);
    }

    // Remove vertices with no incoming particles or no outgoing particles
    for (auto ivtx : m_vertices) {
        int barcode = ivtx.first;
        GenVertexPtr vtx = ivtx.second;
        if ( vtx->particles_in().empty() ) {
            HEPMC3_DEBUG(30, "ReaderAsciiHepMC2::read_event "
                         << "- found a vertex without incoming particles: "
                         << barcode );
            //Sometimes the root vertex has no incoming particles.  Here we try to save the event.
            std::vector<GenParticlePtr> beams;
            beams.reserve(2);
            for (const auto& p: vtx->particles_out()) if (p->status() == 4 && !(p->end_vertex())) beams.emplace_back(p);
            for (auto& p: beams)
            {
                vtx->add_particle_in(p);
                vtx->remove_particle_out(p);
                HEPMC3_DEBUG(30, "ReaderAsciiHepMC2::read_event - moved particle with status=4 from the outgoing to the incoming particles of vertex: " << barcode);
            }
            if (beams.empty()) {
                HEPMC3_DEBUG(30, "ReaderAsciiHepMC2::read_event - removed vertex without incoming particles: " << barcode );
                evt.remove_vertex(vtx);
                // m_vertices.erase(barcode);
            }
        }
        else if ( vtx->particles_out().empty() ) {

            evt.remove_vertex(vtx);
            // m_vertices.erase(barcode);
            HEPMC3_DEBUG(30, "ReaderAsciiHepMC2::read_event - removed vertex without outgoing particles: " << barcode);
        }
    }

    evt.check_cycles();
    
    if (m_options.count("event_random_states_are_separated") != 0)
    {
        std::shared_ptr<VectorLongIntAttribute> random_states_a = evt.attribute<VectorLongIntAttribute>("random_states");
        if (random_states_a) {
            std::vector<long int> random_states_v = random_states_a->value();
            for (size_t i = 0; i < random_states_v.size(); ++i ) {
                evt.add_attribute("random_states" + std::to_string((long long unsigned int)i), std::make_shared<IntAttribute>(random_states_v[i]));
            }
            evt.remove_attribute("random_states");
        }

    }

    std::map< std::string, std::map<int, std::shared_ptr<Attribute> > > cached_attributes = evt.attributes();

    for (auto iattr : cached_attributes) {
        std::string name  = iattr.first;
        auto        attrs = iattr.second;

        for (auto jattr : attrs) {
            int  id   = jattr.first;
            auto attr = jattr.second;

            GenParticlePtr part = nullptr;
            GenVertexPtr   vtx  = nullptr;
            if (id < 0) {
                auto iter = m_vertices.find(id);
                if (iter != m_vertices.end()) vtx = iter->second;
            }
            if (id > 0) {
                auto iter = m_particles.find(id);
                if (iter != m_particles.end()) part = iter->second;
            }

            if (name == "flows" and part and
                    m_options.count("particle_flows_are_separated") != 0) {
                std::shared_ptr<VectorIntAttribute> v =
                    std::dynamic_pointer_cast<VectorIntAttribute>(attr);
                if (v) {
                    for (size_t i = 0; i < v->value().size(); i++)
                        part->add_attribute("flow"+std::to_string(i+1),
                                            std::make_shared<IntAttribute>(v->value()[i]));
                }
            }
            else if (name == "weights" and vtx and
                     m_options.count("vertex_weights_are_separated") != 0) {
                std::shared_ptr<VectorDoubleAttribute> v =
                    std::dynamic_pointer_cast<VectorDoubleAttribute>(attr);
                if (v) {
                    for (size_t i = 0; i < v->value().size(); i++)
                        vtx->add_attribute("flow"+std::to_string(i+1),
                                           std::make_shared<DoubleAttribute>(v->value()[i]));
                }
            }
            else if (part)
                part->add_attribute(name, attr);
            else if (vtx)
                vtx->add_attribute(name, attr);
        }
    }
    return true;
}

int ReaderAsciiHepMC2::parse_event_information(GenEvent &evt, const char *buf) {
    const char          *cursor             = buf;
    size_t               vertices_count     = 0;
    int                  random_states_size = 0;
    int                  weights_size       = 0;
    std::vector<long>    random_states(0);
    std::vector<double>  weights(0);

    // event number
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.set_event_number(atoi(cursor));

    //mpi
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("mpi", std::make_shared<IntAttribute>(atoi(cursor)));

    //event scale
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("event_scale", std::make_shared<DoubleAttribute>(atof(cursor)));

    //alpha_qcd
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("alphaQCD", std::make_shared<DoubleAttribute>(atof(cursor)));

    //alpha_qed
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("alphaQED", std::make_shared<DoubleAttribute>(atof(cursor)));

    //signal_process_id
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("signal_process_id", std::make_shared<IntAttribute>(atoi(cursor)));

    //signal_process_vertex
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    evt.add_attribute("signal_process_vertex", std::make_shared<IntAttribute>(atoi(cursor)));

    // num_vertices
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    vertices_count = atoi(cursor);

    // SKIPPED: beam 1
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;

    // SKIPPED: beam 2
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;

    //random states
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    random_states_size = atoi(cursor);
    if (random_states_size >= 0 ) {
        random_states.resize(random_states_size);
    } else {
        HEPMC3_DEBUG(0, "ReaderAsciiHepMC2: E: " << evt.event_number()
                     << " (" << vertices_count << "V, "
                     << random_states_size << "RS)")
    }
    for ( int i = 0; i < random_states_size; ++i ) {
        if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
        random_states[i] = atoi(cursor);
    }

    if (!random_states.empty()) evt.add_attribute("random_states", std::make_shared<VectorLongIntAttribute>(random_states));

    // weights
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    weights_size = atoi(cursor);
    if (weights_size >= 0 ) {
        weights.resize(weights_size);
    } else {
        HEPMC3_DEBUG(0, "ReaderAsciiHepMC2: E: " << evt.event_number()
                     << " (" << vertices_count << "V, "
                     << weights_size << "WS)")
    }
    for ( int i = 0; i < weights_size; ++i ) {
        if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
        weights[i] = atof(cursor);
    }

    evt.weights() = weights;

    HEPMC3_DEBUG(10, "ReaderAsciiHepMC2: E: " << evt.event_number()
                 << " (" << vertices_count << "V, "
                 << weights_size << "W, " << random_states_size << "RS)")

    return vertices_count;
}

bool ReaderAsciiHepMC2::parse_units(GenEvent &evt, const char *buf) {
    const char *cursor  = buf;

    // momentum
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;
    Units::MomentumUnit momentum_unit = Units::momentum_unit(cursor);

    // length
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;
    Units::LengthUnit length_unit = Units::length_unit(cursor);

    evt.set_units(momentum_unit, length_unit);

    HEPMC3_DEBUG(10, "ReaderAsciiHepMC2: U: "
                 << Units::name(evt.momentum_unit()) << " "
                 << Units::name(evt.length_unit()))

    return true;
}

int ReaderAsciiHepMC2::parse_vertex_information(GenEvent& evt, const char *buf) {
    GenVertexPtr  data = std::make_shared<GenVertex>();
    const char   *cursor            = buf;
    int           barcode           = 0;
    int           num_particles_out = 0;
    int                  weights_size       = 0;
    std::vector<double>  weights(0);
    // barcode
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    barcode = atoi(cursor);

    // status
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    data->set_status(atoi(cursor));

    // x
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double X(atof(cursor));

    // y
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double Y(atof(cursor));

    // z
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double Z(atof(cursor));

    // t
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double T(atof(cursor));
    data->set_position(FourVector(X,Y,Z,T));

    // SKIPPED: num_orphans_in
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;

    // num_particles_out
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    num_particles_out = atoi(cursor);

    //  weights
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    weights_size = atoi(cursor);
    weights.resize(weights_size);

    for ( int i = 0; i < weights_size; ++i ) {
        if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
        weights[i] = atof(cursor);
    }

    evt.add_vertex(data,barcode);
    HEPMC3_DEBUG(10, "ReaderAsciiHepMC2::parse_vertex_information "
                 << "- added vertex " << data->id() << " (hint "
                 << barcode << ")");
    m_vertices[barcode] = data;
    m_last_vertex = data;

    if (!weights.empty()) data->add_attribute("weights", std::make_shared<VectorDoubleAttribute>(weights));

    return num_particles_out;
}

int ReaderAsciiHepMC2::parse_particle_information(GenEvent& evt, const char *buf) {
    GenParticlePtr  data = std::make_shared<GenParticle>();
    const char     *cursor  = buf;
    int             end_vtx = 0;

    /// @note barcode is ignored
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    int barcode = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    int pid = atoi(cursor);
    data->set_pid(pid);

    // px
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double Px(atof(cursor));

    // py
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double Py(atof(cursor));

    // pz
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double Pz(atof(cursor));

    // pe
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double E(atof(cursor));
    data->set_momentum(FourVector(Px,Py,Pz,E));

    // m
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    data->set_generated_mass(atof(cursor));

    // status
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    data->set_status(atoi(cursor));

    //theta
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double theta_v = atof(cursor);
    if (theta_v != 0.0)
        evt.add_attribute("theta",
                          std::make_shared<DoubleAttribute>(theta_v),
                          data);

    //phi
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    double phi_v = atof(cursor);
    if (phi_v != 0.0)
        evt.add_attribute("phi",
                          std::make_shared<DoubleAttribute>(phi_v),
                          data);

    // end_vtx_code
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    end_vtx = atoi(cursor);

    //flow
    if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
    int flowsize = atoi(cursor);

    std::map<int, int> flows;
    for (int i = 0; i < flowsize; i++)
    {
        if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
        int  flowindex = atoi(cursor);
        if ( !(cursor = strchr(cursor+1, ' ')) ) return -1;
        int flowvalue = atoi(cursor);
        flows[flowindex] = flowvalue;
    }
    // Set prod_vtx link
    if (not m_last_vertex) {
        HEPMC3_DEBUG(1, "The first particle in event appears before the first vertex");
        return -1;
    }
    if ( end_vtx == m_last_vertex->id() ) {
        m_last_vertex->add_particle_in(data);
        end_vtx = 0;
    }
    else {
        m_last_vertex->add_particle_out(data);
    }

    if (end_vtx != 0) {
        auto iter = m_vertices.find(end_vtx);
        if (iter == m_vertices.end())
            m_forward_mothers[data] = end_vtx;
        else
            iter->second->add_particle_in(data);
    }

    evt.add_particle(data,barcode);
    m_particles[barcode] = data;

    if (flowsize)
    {
        std::vector<int> vectorflows;
        vectorflows.reserve(flows.size());
        for (const auto& f: flows) { vectorflows.emplace_back(f.second); }
        evt.add_attribute("flows",
                          std::make_shared<VectorIntAttribute>(vectorflows),
                          data);
    }
    
    return 0;
}

bool ReaderAsciiHepMC2::parse_xs_info(GenEvent &evt, const char *buf) {
    const char *cursor  = buf;
    std::shared_ptr<GenCrossSection>  xs     = std::make_shared<GenCrossSection>();

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    const double xs_val  = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    const double xs_err = atof(cursor);
    const size_t all = m_options.count("disable_pad_cross_sections") ? size_t{1} :
                       std::max(evt.weights().size(),size_t{1});
    const double xs_val_dummy = m_options.count("pad_cross_section_value") ? std::strtod(m_options.at("pad_cross_section_value").c_str(),nullptr) : 0.0;
    const double xs_err_dummy = m_options.count("pad_cross_section_error") ? std::strtod(m_options.at("pad_cross_section_error").c_str(),nullptr) : 0.0;
    xs->set_cross_section(std::vector<double>(all,xs_val_dummy), std::vector<double>(all,xs_err_dummy));
    xs->set_xsec(0,xs_val);
    xs->set_xsec_err(0,xs_err);
    evt.add_attribute("GenCrossSection", xs);
    return true;
}

bool ReaderAsciiHepMC2::parse_weight_names(const char *buf) {
    const char     *cursor  = buf;
    const char     *cursor2 = buf;
    int             w_count = 0;
    std::vector<std::string>  w_names;

    // Ignore weight names if no GenRunInfo object
    if ( !run_info() ) return true;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    w_count = atoi(cursor);

    if ( w_count <= 0 ) return false;

    w_names.resize(w_count);

    for ( int i=0; i < w_count; ++i ) {
        // Find pair of '"' characters
        if ( !(cursor  = strchr(cursor+1, '"')) ) return false;
        if ( !(cursor2 = strchr(cursor+1, '"')) ) return false;

        // Strip cursor of leading '"' character
        ++cursor;

        w_names[i].assign(cursor, cursor2-cursor);

        cursor = cursor2;
    }

    run_info()->set_weight_names(w_names);

    return true;
}

bool ReaderAsciiHepMC2::parse_heavy_ion(GenEvent &evt, const char *buf) {
    std::shared_ptr<GenHeavyIon>  hi     = std::make_shared<GenHeavyIon>();
    const char              *cursor = buf;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Ncoll_hard = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Npart_proj = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Npart_targ = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Ncoll = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->spectator_neutrons = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->spectator_protons = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->N_Nwounded_collisions = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Nwounded_N_collisions = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->Nwounded_Nwounded_collisions = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->impact_parameter = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->event_plane_angle = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->eccentricity = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    hi->sigma_inel_NN = atof(cursor);

    // Not in HepMC2:
    hi->centrality = 0.0;

    evt.add_attribute("GenHeavyIon", hi);

    return true;
}

bool ReaderAsciiHepMC2::parse_pdf_info(GenEvent &evt, const char *buf) {
    std::shared_ptr<GenPdfInfo>  pi     = std::make_shared<GenPdfInfo>();
    const char             *cursor = buf;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->parton_id[0] = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->parton_id[1] = atoi(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->x[0] = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->x[1] = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->scale = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->xf[0] = atof(cursor);

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    pi->xf[1] = atof(cursor);

    //For compatibility with original HepMC2
    bool pdfids = true;
    if ( !(cursor = strchr(cursor+1, ' ')) ) pdfids = false;
    if (pdfids) {pi->pdf_id[0] = atoi(cursor);}
    else  {pi->pdf_id[0] = 0;}

    if (pdfids) if ( !(cursor = strchr(cursor+1, ' ')) )  pdfids = false;
    if (pdfids) { pi->pdf_id[1] = atoi(cursor);}
    else  {pi->pdf_id[1] = 0;}

    evt.add_attribute("GenPdfInfo", pi);

    return true;
}
bool ReaderAsciiHepMC2::failed() { return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate(); }

void ReaderAsciiHepMC2::close() {
    if ( !m_file.is_open() ) return;
    m_file.close();
}

} // namespace HepMC3
