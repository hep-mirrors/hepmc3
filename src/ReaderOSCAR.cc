// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR.cc
/// @brief Implementation of \b class ReaderOSCAR
///
#include <cstring>
#include <sstream>

#include "HepMC3/ReaderOSCAR.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"

namespace HepMC3 {


ReaderOSCAR::ReaderOSCAR(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR::ReaderOSCAR(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}



ReaderOSCAR::~ReaderOSCAR() { if (!m_isstream) close(); }

bool ReaderOSCAR::skip(const int n)
{

    return true;
}


bool ReaderOSCAR::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;

    evt.clear();
    evt.set_run_info(run_info());

    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);

        if ( strlen(buf) == 0 ) continue;
        // Check for ReaderOSCAR header/footer
        if ( strncmp(buf, "# ", 2) == 0 ) {
            if ( strncmp(buf, "# OSC1999A", 10) == 0 ) {
                HEPMC3_WARNING("ReaderOSCAR: So far OSCAR format is not supported. Will close the input after processing the header.")
                m_header.clear();
            } else {
                m_header.push_back(std::string(buf));
            }
            continue;
        }
        if ( strncmp(buf, "0 0 ", 4) == 0 ) {
            if ( !(cursor = strchr(cursor+4, ' ')) ) return false;
            evt.set_event_number(atoi(cursor));
            if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
            double impact_parameter = atof(cursor);
            evt.add_attribute("impact_parameter", std::make_shared<DoubleAttribute>(impact_parameter));
            end_event();

            cursor = buf;
            if (m_vertices.empty()) continue;
            evt.add_vertex(m_vertices.front());
            continue;
        }
        int nin = atoi(cursor);
        if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
        int nout = atoi(cursor);
        m_interaction.clear();
        m_interaction.push_back(std::string(buf));
        for (int i=0; i<nin+nout; i++ ) {
            m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
            m_interaction.push_back(std::string(buf));
        }
        parse_interaction();
    }
    return true;
}


GenParticlePtr ReaderOSCAR::parse_particle(const std::string& s) {

    GenParticlePtr p = std::make_shared<GenParticle>();
    return p;
}

void ReaderOSCAR::parse_interaction() {
    GenVertexPtr v = std::make_shared<GenVertex>();
    char buf[512];
    sprintf(buf,"%s\n",m_interaction.at(0).c_str());
    const char                 *cursor   = buf;
    int nin = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return;
    int nout = atoi(cursor);
    printf("%i %i %i\n", nin, nout, m_interaction.size());
    for (int i=1; i<nin+1; i++ ) v->add_particle_in(parse_particle(m_interaction.at(i)));
    for (int i=nin+1; i<nout+nin+1; i++ ) v->add_particle_out(parse_particle(m_interaction.at(i)));
    m_vertices.push_back(v);
}
void ReaderOSCAR::end_event() {
    parse_header();


}
void ReaderOSCAR::parse_header() {
    if (m_header.empty()) return;
    if (m_header.size() > 2 && m_header[2].length() > 2) {
        struct GenRunInfo::ToolInfo generator = {m_header[2].substr(2), "0", std::string("Used generator")};
        run_info()->tools().push_back(generator);
    }
}

bool ReaderOSCAR::failed() { return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate(); }

void ReaderOSCAR::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}


} // namespace HepMC3
