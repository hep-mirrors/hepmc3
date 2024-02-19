// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2024 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR1997.cc
/// @brief Implementation of \b class ReaderOSCAR1997
///
#include <cstring>
#include <sstream>
#include <cassert>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"
#include "HepMC3/ReaderOSCAR1997.h"


namespace HepMC3 {

std::vector<std::string> tokenize_string(const std::string& str, const std::string& delimiters );

ReaderOSCAR1997::ReaderOSCAR1997(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR1997: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1997::ReaderOSCAR1997(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR1997: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1997::ReaderOSCAR1997(std::shared_ptr<std::istream> s_stream)
    : m_shared_stream(s_stream), m_stream(s_stream.get()), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR1997: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1997::~ReaderOSCAR1997() {
    if (!m_isstream) close();
}

bool ReaderOSCAR1997::skip(const int n)
{
    /// So far this is not implemented
    return false;
}

inline void filter_spaces(char* actual_input) {
    const char* input = actual_input;
    char* output = actual_input;
    ssize_t j = 0;
    ssize_t l = strlen(input);
    if (!l) return;
    for (ssize_t i = 1; i < l; i++) {
        if ( !std::isspace(input[i]) || !std::isspace(output[j]))
        { j++; output[j] = input[i]; }
    }
    j++;
    output[j] = '\0';
}

bool ReaderOSCAR1997::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    /// The OSCAR format has fixed units
    evt.set_run_info(run_info());
    bool event_header = false;
    int n_particles_expected = 0;
    int n_particles_parsed = 0;
    /// Create a single interaction vertex
    GenVertexPtr top = std::make_shared<GenVertex>();
    /// Create and add dummy beam particles
    GenParticlePtr b1 = std::make_shared<GenParticle>();
    GenParticlePtr b2 = std::make_shared<GenParticle>();
    evt.add_particle(b1);
    evt.add_particle(b2);
    top->add_particle_in(b1);
    top->add_particle_in(b2);
    evt.add_vertex(top);
    evt.add_beam_particle(b1);
    evt.add_beam_particle(b2);
    /// OSCAR has fixed units fm/GEV, right?
    const double from_fm = (evt.length_unit() == Units::MM) ? 1e-12 : 1e-13;
    const double from_gev = (evt.momentum_unit() == Units::GEV) ? 1 : 1e+3;
    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 ) continue;
        /// Check if the event header was parsed.
        if (event_header) {
            /// Yes, the expected content is particle
            filter_spaces(buf);
            int i[2];
            double d[9];
            sscanf(buf," %i %i %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", i, i+1, d, d+1, d+2, d+3, d+4, d+5, d+6, d+7, d+8);
            /// Create two particles. One for the final state and the other to connect the "interaction" vertex with the "final" vertex
            GenParticlePtr in = std::make_shared<GenParticle>(FourVector(from_gev*d[0], from_gev*d[1], from_gev*d[2], from_gev*d[3]), i[1], 3);
            GenParticlePtr out = std::make_shared<GenParticle>(FourVector(from_gev*d[0], from_gev*d[1], from_gev*d[2], from_gev*d[3]), i[1], 1);
            in->set_generated_mass(d[4]);
            out->set_generated_mass(d[4]);
            /// Create "final" vertex
            GenVertexPtr v = std::make_shared<GenVertex>(FourVector(from_fm*d[5], from_fm*d[6], from_fm*d[7], from_fm*d[8]));
            v->add_particle_in(in);
            v->add_particle_out(out);
            top->add_particle_out(in);
            evt.add_vertex(v);
            n_particles_parsed++;
            /// If all the particles are present, return.
            if (n_particles_parsed >= n_particles_expected) {
                return true;
            }
            continue;
        }

        /// The very beginning of input
        if ( strncmp(buf, "OSC1997A", 8) == 0 ) {
            m_header.clear();
            continue;
        }
        /// The expected content is file header.
        if (m_header.size() < 2) { m_header.push_back(std::string(buf)); continue;}
        else if (m_header.size() == 2) {
            /// We add the line that describes the content to the run info. That should be "final_id_p_x".
            run_info()->add_attribute("content", std::make_shared<StringAttribute>(m_header.at(0)));
            /// In the second line of header we locate the reaction decription
            std::string toparse1 = m_header.at(1);
            size_t reaction_b = toparse1.find_first_of("(");
            size_t reaction_e = toparse1.find_last_of(")") + 1;
            if (reaction_e > reaction_b + 1) {
                std::string reaction = toparse1.substr(reaction_b, reaction_e - reaction_b);
                /// Add reaction desctiption to run info
                run_info()->add_attribute("reaction", std::make_shared<StringAttribute>(reaction));
                toparse1.erase(reaction_b,reaction_e - reaction_b);
                /// We remove the reaction decription before we tokenize the line. The - is needed for SMASH that concatenates the name and the version of generator.
                std::vector<std::string> parsed1 = tokenize_string(toparse1, "- \r\n");
                struct GenRunInfo::ToolInfo generator = { parsed1.size() > 1 ? parsed1.at(0) :"Unknown", parsed1.size() > 2 ? parsed1.at(1) : "0.0.0", std::string("Used generator")};
                /// Add generator name to run info
                run_info()->tools().push_back(generator);
                /// Add more physical attributes
                if (parsed1.size() > 2) run_info()->add_attribute("reference_frame", std::make_shared<StringAttribute>(parsed1.at(2)));
                if (parsed1.size() > 3) run_info()->add_attribute("beam_energy", std::make_shared<DoubleAttribute>(std::strtof(parsed1.at(3).c_str(),NULL)));
                if (parsed1.size() > 4) run_info()->add_attribute("test_particles_per_nuclon", std::make_shared<IntAttribute>(std::atoi(parsed1.at(4).c_str())));
            }
            /// Set a name for one (the only) weight
            std::vector<std::string> weightnames;
            weightnames.push_back("Default");
            run_info()->set_weight_names(weightnames);
        }
        if (!event_header) {
            /// A header of new event is expected.
            GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
            hi->set(-1, -1, -1, -1, -1, -1,
                    -1, -1, -1,
                    -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
            GenCrossSectionPtr cs = std::make_shared<GenCrossSection>();
            /// Parse the line to extract HI information and the number of expected particles
            while (cursor && *cursor == ' ') cursor++;
            evt.set_event_number(atoi(cursor));
            cursor = strchr(cursor + 1, ' ');
            while (cursor && *cursor == ' ') cursor++;
            n_particles_expected = atoi(cursor);
            while (cursor && *cursor == ' ') cursor++;
            cursor = strchr(cursor + 1, ' ');
            double impact_parameter = atof(cursor);
            hi->impact_parameter = impact_parameter;
            while (cursor && *cursor == ' ') cursor++;
            cursor = strchr(cursor + 1, ' ');
            double event_plane_angle = atof(cursor);
            hi->event_plane_angle = event_plane_angle;
            /// Event header was parsed
            event_header = true;
            /// Set dummy XS info
            cs->set_cross_section(1.0, 1.0);
            evt.set_cross_section(cs);
            evt.set_heavy_ion(hi);
            /// Set weights
            evt.weights() = std::vector<double>(1,1);
            /// Attach the only vertex to the event
            evt.add_vertex(top);
            continue;
        }
    }

    return true;
}

bool ReaderOSCAR1997::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR1997::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
