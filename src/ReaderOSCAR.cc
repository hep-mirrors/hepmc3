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
    evt.clear();
    evt.set_run_info(run_info());

    //
    // Parse event, vertex and particle information
    //
    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);

        if ( strlen(buf) == 0 ) continue;
        // Check for ReaderOSCAR header/footer
        if ( strncmp(buf, "# OSC1999A", 10) == 0 ) {
                HEPMC3_WARNING("ReaderOSCAR: So far OSCAR format is not supported. Will close the input.")
                std::cout << buf << std::endl;
                m_isstream ? m_stream->clear(std::ios::eofbit) : m_file.clear(std::ios::eofbit);
        }
    }
    return true;
}


bool ReaderOSCAR::failed() { return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate(); }

void ReaderOSCAR::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}


} // namespace HepMC3
