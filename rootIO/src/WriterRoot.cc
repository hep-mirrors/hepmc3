// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterRoot.cc
 *  @brief Implementation of \b class WriterRoot
 *
 */
#include "HepMC/WriterRoot.h"
#include <cstdio>  // sprintf

namespace HepMC {

WriterRoot::WriterRoot(const std::string &filename):
m_file(filename.c_str(),"RECREATE"),
m_events_count(0) {

    if ( !m_file.IsOpen() ) {
        ERROR( "WriterRoot: problem opening file: " << filename )
        return;
    }
}

void WriterRoot::write_event(const GenEvent &evt) {
    if ( !m_file.IsOpen() ) return;

    GenEventData data;
    evt.write_data(data);

    char buf[16] = "";
    sprintf(buf,"%15i",++m_events_count);

    int nbytes = m_file.WriteObject(&data, buf);

    if( nbytes == 0 ) {
        ERROR( "WriterRoot: error writing event")
        m_file.Close();
    }
}

void WriterRoot::close() {
    m_file.Close();
}

bool WriterRoot::failed() {
    if ( !m_file.IsOpen() ) return true;

    return false;
}

} // namespace HepMC
