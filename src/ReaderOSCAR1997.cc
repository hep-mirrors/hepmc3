// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR1997.cc
/// @brief Implementation of \b class ReaderOSCAR1997
///
#include <cstring>
#include <sstream>
#include <cassert>
#include <regex>

#include "HepMC3/ReaderOSCAR1997.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"

namespace HepMC3 {


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

ReaderOSCAR1997::~ReaderOSCAR1997() {
    if (!m_isstream) close();
}

bool ReaderOSCAR1997::skip(const int n)
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
    output[j++] = '\0';
}
bool ReaderOSCAR1997::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    evt.set_run_info(run_info());
    bool event_header = false;
    int n_particles_expected = 0;
    int n_particles_parsed = 0;
    GenVertexPtr top = std::make_shared<GenVertex>();
    				GenParticlePtr b =std::make_shared<GenParticle>();
				evt.add_particle(b);
				top->add_particle_in(b);
				evt.add_vertex(top);
				evt.add_beam_particle(b);
    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 ) continue;
        if (event_header) {
            filter_spaces(buf);
            int i[2];
            double d[9];
            sscanf(buf," %i %i %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", i, i+1, d, d+1, d+2, d+3, d+4, d+5, d+6,  d+7,  d+8);
            GenParticlePtr in = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], d[3]),i[1],3);
            GenParticlePtr out = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], d[3]),i[1],1);
            in->set_generated_mass(d[4]);
            out->set_generated_mass(d[4]);
            GenVertexPtr v = std::make_shared<GenVertex>(FourVector(d[5], d[6],  d[7],  d[8]));
            v->add_particle_in(in);
            v->add_particle_out(out);
            top->add_particle_out(in);
            evt.add_vertex(v);
            n_particles_parsed++;
            if (n_particles_parsed >= n_particles_expected) {

				 return true;
            }
            continue;
        }

        if ( strncmp(buf, "OSC1997A", 8) == 0 ) {
            HEPMC3_WARNING("ReaderOSCAR1997: So far OSCAR1997 format is not fully supported. ")
            m_header.clear();
            continue;
        }
        if (m_header.size() < 2) { m_header.push_back(std::string(buf)); continue;}
        else if (m_header.size() == 2) {

            std::string toparse = m_header.at(1);
            size_t reaction_b = toparse.find_first_of("(");
            size_t reaction_e = toparse.find_last_of(")")+1;
            std::string reaction = toparse.substr(reaction_b,reaction_e-reaction_b);
            toparse.erase(reaction_b,reaction_e-reaction_b);
            std::vector<std::string> parsed;
            while (toparse.size()>0)
            {
                toparse.erase(0,toparse.find_first_not_of(" "));
                parsed.push_back(toparse.substr(0,toparse.find_first_of(" ")));
                toparse.erase(0,parsed.back().size());
            }
            printf("%s  %s %s  %s  %f  %i\n",parsed.at(0).c_str(),parsed.at(1).c_str(), reaction.c_str(),
                   parsed.at(2).c_str(), std::strtof(parsed.at(3).c_str(),NULL), std::atoi(parsed.at(4).c_str()));
    struct GenRunInfo::ToolInfo generator={parsed.at(0),parsed.at(1),std::string("")};
    run_info()->tools().push_back(generator);
 
        }
        if (!event_header) {
            while (cursor && *cursor == ' ') cursor++;
            evt.set_event_number(atoi(cursor));
            cursor = strchr(cursor+1, ' ');
            while (cursor && *cursor == ' ') cursor++;
            //cursor = strchr(cursor+1, ' ');
            n_particles_expected= atoi(cursor);
           // printf("--->%s<-->%i \n",buf,n_particles_expected);
            while (cursor && *cursor == ' ') cursor++;
            cursor = strchr(cursor+1, ' ');
            double impact_parameter = atof(cursor);
            evt.add_attribute("impact_parameter", std::make_shared<DoubleAttribute>(impact_parameter));
            event_header=true;
            evt.add_vertex(top);
        }
    }

    return true;
}


void ReaderOSCAR1997::parse_header() {
    if (m_header.empty()) return;
    for (auto &s: m_header) s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
    if (m_header.size() > 2 && m_header[1].length() > 2) {
        struct GenRunInfo::ToolInfo generator = {m_header[1].substr(2), "0", std::string("Used generator")};
        auto ri = run_info();
        ri->tools().push_back(generator);
        std::vector<std::string> weightnames;
        weightnames.push_back("Default");
        ri->set_weight_names(weightnames);
        set_run_info(ri);
    }
}

bool ReaderOSCAR1997::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR1997::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
