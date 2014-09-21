/**
 *  @file IO_FileBase.cc
 *  @brief Partial implementation of \b abstract \b class IO_FileBase
 *
 */
#include <iostream>
#include <fstream>
#include "HepMC/IO/IO_FileBase.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Setup.h"
using std::ostream;
using std::endl;

namespace HepMC {

IO_FileBase::IO_FileBase(const std::string& filename, std::ios::openmode mode):
m_file(filename.c_str(),mode | std::ios::binary) {

    m_mode = mode;

    if( m_mode != std::ios::in && m_mode != std::ios::out ) {
        ERROR( "IO_FileBase: only ios::in and ios::out modes are supported" )
        m_file.clear(std::ios::badbit);
        return;
    }

    if( !m_file.is_open() ) {
        ERROR( "IO_FileBase: couldn't open file: " << filename )
        m_file.clear(std::ios::badbit);
        return;
    }
    if( m_mode == std::ios::out ) {
        m_file<<"HepMC::Version 3.00.00"<<endl;
        m_file<<"HepMC::IO_GenEvent-START_EVENT_LISTING"<<endl;
    }
}

IO_FileBase::~IO_FileBase() {
    close();
}

void IO_FileBase::close() {
    if( !m_file.rdstate() && m_mode == std::ios::out ) {
        m_file << "HepMC::IO_GenEvent-END_EVENT_LISTING" << endl << endl;
    }
    m_file.close();

    // Just in case ...
    m_file.clear(std::ios::badbit);
}

} // namespace HepMC
