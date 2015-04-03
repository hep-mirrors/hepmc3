// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderRoot.cc
 *  @brief Implementation of \b class ReaderRoot
 *
 */
#include "HepMC/ReaderRoot.h"

namespace HepMC {

ReaderRoot::ReaderRoot(const std::string &filename):
m_file(filename.c_str()),
m_next(m_file.GetListOfKeys()) {

    if ( !m_file.IsOpen() ) {
        ERROR( "ReaderRoot: problem opening file: " << filename )
        return;
    }
}

bool ReaderRoot::read_event(GenEvent& evt) {

    TKey *key = (TKey*)m_next();

    if( !key ) {
        m_file.Close();
        return false;
    }

    GenEventData *data = (GenEventData*)key->ReadObj();

    if( !data ) {
        ERROR("ReaderRoot: could not read event from root file")
        m_file.Close();
        return false;
    }

    evt.read_data(*data);

    delete data;
    return true;
}

void ReaderRoot::close() {
    m_file.Close();
}

bool ReaderRoot::failed() {
    if ( !m_file.IsOpen() ) return true;

    return false;
}

} // namespace HepMC
