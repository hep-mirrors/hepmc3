#include <iostream>
#include <fstream>
#include "HepMC3/IO_Base.h"
#include "HepMC3/Log.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

IO_Base::IO_Base(const std::string& filename, std::ios::openmode mode):
m_file(filename.c_str(),mode),
m_io_error_state(0) {

    m_mode = mode;

    if( m_mode != std::ios::in && m_mode != std::ios::out ) {
        ERROR( "IO_Base: only ios::in and ios::out modes are supported" )
        m_io_error_state = 1;
        return;
    }

    if( !m_file.is_open() ) {
        ERROR( "IO_Base: couldn't open file: " << filename )
        m_io_error_state = 1;
        return;
    }
    if( m_mode == std::ios::out ) {
        m_file<<"HepMC::Version 3.00.00"<<endl;
        m_file<<"HepMC::IO_GenEvent-START_EVENT_LISTING"<<endl;
    }

    m_io_error_state = 0;
}

IO_Base::~IO_Base() {
    close();
}

void IO_Base::close() {
    if( !m_io_error_state && !m_file.rdstate() && m_mode == std::ios::out ) {
        m_file << "HepMC::IO_GenEvent-END_EVENT_LISTING" << endl << endl;
    }
    m_file.close();
    m_io_error_state = 1;
}

} // namespace HepMC3
