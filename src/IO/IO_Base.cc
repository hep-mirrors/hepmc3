/**
 *  @file IO_Base.cc
 *  @brief Partial implementation of \b abstract \b class HepMC3::IO_Base
 *
 */
#include <iostream>
#include <fstream>
#include "HepMC3/IO/IO_Base.h"
#include "HepMC3/Setup.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

IO_Base::IO_Base(const std::string& filename, std::ios::openmode mode):
m_file(filename.c_str(),mode | std::ios::binary) {

    m_mode = mode;

    if( m_mode != std::ios::in && m_mode != std::ios::out ) {
        ERROR( "IO_Base: only ios::in and ios::out modes are supported" )
        m_file.clear(std::ios::badbit);
        return;
    }

    if( !m_file.is_open() ) {
        ERROR( "IO_Base: couldn't open file: " << filename )
        m_file.clear(std::ios::badbit);
        return;
    }
    if( m_mode == std::ios::out ) {
        m_file<<"HepMC::Version 3.00.00"<<endl;
        m_file<<"HepMC::IO_GenEvent-START_EVENT_LISTING"<<endl;
    }
}

IO_Base::~IO_Base() {
    close();
}

void IO_Base::close() {
    if( !m_file.rdstate() && m_mode == std::ios::out ) {
        m_file << "HepMC::IO_GenEvent-END_EVENT_LISTING" << endl << endl;
    }
    m_file.close();

    // Just in case ...
    m_file.clear(std::ios::badbit);
}

} // namespace HepMC3
