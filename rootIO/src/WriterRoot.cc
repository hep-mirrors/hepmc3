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
m_file(filename.c_str(),"RECREATE") {

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
    sprintf(buf,"%15i",evt.event_number());

    int nbytes = m_file.WriteObject(&data, buf);

    if( nbytes == 0 ) {
        ERROR( "WriterRoot: error writing event")
        m_file.Close();
    }
}

void WriterRoot::close() {
    m_file.Close();
}

int WriterRoot::rdstate() {
    if ( !m_file.IsOpen() ) return 1;

    return 0;
}

} // namespace HepMC
