// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR2013.cc
/// @brief Implementation of \b class ReaderOSCAR2013
///
#include <cstring>
#include <sstream>
#include <cassert>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"
#include "HepMC3/ReaderOSCAR2013.h"


namespace HepMC3 {


ReaderOSCAR2013::ReaderOSCAR2013(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR2013: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR2013::ReaderOSCAR2013(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR2013: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR2013::~ReaderOSCAR2013() {
    if (!m_isstream) close();
}

bool ReaderOSCAR2013::skip(const int n)
{
    return true;
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
inline std::vector<std::string> tokenize_string(const std::string& str, const std::string& delimiters )
{
    std::vector<std::string> tokens;
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
}
bool ReaderOSCAR2013::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    evt.set_units(Units::GEV,Units::MM);//Fixed units?
    evt.set_run_info(run_info());
    bool file_header_parsed = false;
    int n_particles_expected = 0;
    int n_particles_parsed = 0;
    GenVertexPtr top = std::make_shared<GenVertex>();
// We add two particles, despite it is actually not needed.
    GenParticlePtr b1 =std::make_shared<GenParticle>();
    evt.add_beam_particle(b1);
    top->add_particle_in(b1);
    GenParticlePtr b2 =std::make_shared<GenParticle>();
    evt.add_beam_particle(b2);
    top->add_particle_in(b2);
    evt.add_vertex(top);
    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 ) continue;
        if ( strncmp(buf, "# event", 7) == 0 ) {
            std::vector<std::string> parsed = tokenize_string(std::string(buf), " #");
            if (parsed.size() == 4) {
                //start of event!
                evt.set_event_number(atoi(parsed.at(1).c_str()));
                n_particles_expected = atoi(parsed.at(3).c_str());
            }
            if (parsed.size() == 8) {
                //end of event!
                GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
                hi->set(-1, -1, -1, -1, -1, -1,
                        -1, -1, -1,
                        -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
                GenCrossSectionPtr cs = std::make_shared<GenCrossSection>();
                hi->impact_parameter = std::strtof(parsed.at(5).c_str(),nullptr);
                cs->set_cross_section(1.0,1.0);
                evt.set_cross_section(cs);
                evt.set_heavy_ion(hi);
                evt.set_run_info(run_info());
                evt.weights() = std::vector<double>(1,1);
                evt.add_vertex(top);
                if ( n_particles_parsed != n_particles_expected ) HEPMC3_ERROR("ReaderOSCAR2013: wrong number of particles parsed"<<n_particles_parsed << "vs expected "<< n_particles_expected);
                break;
            }
            continue;
        }
        if ( strncmp(buf, "#!OSCAR2013", 11) == 0 ) {
            if (strncmp(buf,"#!OSCAR2013 particle_lists", 26) != 0) HEPMC3_ERROR("Only OSCAR2013 particle_lists is supported");
            m_header.clear();
            continue;
        }

        if (m_header.size() < 2) {
            m_header.push_back(std::string(buf));
            continue;
        }
        else if (m_header.size() == 2 && !file_header_parsed) {
            run_info()->add_attribute("units", std::make_shared<StringAttribute>(m_header.at(0)));
            std::vector<std::string> parsed1 = tokenize_string(m_header.at(1), "- #");
            struct GenRunInfo::ToolInfo generator = {parsed1.size()>0?parsed1.at(0):"Unknown", parsed1.size()>1?parsed1.at(1):"0.0.0", std::string("Used generator")};
            run_info()->tools().push_back(generator);
            std::vector<std::string> weightnames;
            weightnames.push_back("Default");
            run_info()->set_weight_names(weightnames);
            file_header_parsed = true;
        }
        int i[4];
        double d[8];
        sscanf(buf,"%i %lf %lf %lf %lf %lf %lf %lf %lf %i %i %i\n", i, d, d+1, d+2, d+3, d+4, d+5, d+6,  d+7, i+1, i+2, i+3);
        double e = std::sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2] +d[3]*d[3]);
        GenParticlePtr in = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], e),i[1],3);
        GenParticlePtr out = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], e),i[1],1);
        in->set_generated_mass(d[3]);
        out->set_generated_mass(d[3]);
        GenVertexPtr v = std::make_shared<GenVertex>(FourVector(d[4], d[5],  d[6],  d[7]));
        v->add_particle_in(in);
        v->add_particle_out(out);
        top->add_particle_out(in);
        evt.add_particle(in);
        evt.add_particle(out);
        evt.add_vertex(v);
        n_particles_parsed++;
    }
    return true;
}

bool ReaderOSCAR2013::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR2013::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
