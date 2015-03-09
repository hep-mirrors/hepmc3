// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file IO_Root.cc
 *  @brief Implementation of \b class IO_Root
 *
 */
#include "HepMC/IO/IO_Root.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Setup.h"
#include "HepMC/foreach.h"

#include <vector>
#include <cstring> // memset
#include <cstdio>  // sprintf
#include <iostream>
using std::vector;

namespace HepMC {

IO_Root::IO_Root(const std::string &filename, std::ios::openmode mode):
m_mode(mode) {

    if ( mode == std::ios::in ) {
        m_file.reset( new TFile(filename.c_str()) );
        m_next.reset( new TIter(m_file->GetListOfKeys()) );
    }
    else if ( mode == std::ios::out ) {
        m_file.reset( new TFile(filename.c_str(),"RECREATE") );
    }
    else {
        ERROR( "IO_FileBase: only ios::in and ios::out modes are supported" )
        return;
    }

    if ( !m_file || !m_file->IsOpen() ) {
        ERROR( "IO_Root:: problem opening file: " << filename )
        return;
    }
}

IO_Root::~IO_Root() {
    close();
}

void IO_Root::write_event(const GenEvent &evt) {
    if ( rdstate() ) return;
    if ( m_mode != std::ios::out ) {
        ERROR( "IO_Root: attempting to write to input file" )
        return;
    }

    GenEventData data;
    evt.write_data(data);

    char buf[16] = "";
    sprintf(buf,"%15i",evt.event_number());

    int nbytes = m_file->WriteObject(&data, buf);

    if( nbytes == 0 ) {
        ERROR( "IO_Root: error writing event")
        m_file->Close();
    }
}

  
bool IO_Root::fill_next_event(GenEvent& evt) {
    if ( m_mode != std::ios::in ) {
        ERROR( "IO_Root: attempting to read from input file" )
        return false;
    }

    if ( !m_next ) {
        m_file->Close();
        return false;
    }

    TKey *key = (TKey*)(*m_next)();

    if( !key ) {
        m_file->Close();
        return false;
    }

    GenEventData *data = (GenEventData*)key->ReadObj();
        
    if( !data ) {
        ERROR("IO_Root: could not read event from root file")
        m_file->Close();
        return false;
    }   

    evt.read_data(*data);
    
    return true;
}

void IO_Root::close() {
    m_file->Close();
}

std::ios::iostate IO_Root::rdstate() {
    if ( ((bool)m_file) && m_file->IsOpen() ) return std::ios::goodbit;
    else                                      return std::ios::badbit;
}

} // namespace HepMC
