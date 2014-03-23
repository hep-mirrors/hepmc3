#include <iostream>
#include <fstream>
#include "HepMC3/IO_Base.h"
using std::ostream;
using std::cout;
using std::endl;

namespace HepMC3 {

IO_Base::IO_Base(const std::string& filename, std::ios::openmode mode):
m_file(filename.c_str(),mode),
m_io_error_state(0) {

    m_mode = mode;

    if( m_mode != std::ios::in && m_mode != std::ios::out ) {
        cout<<"err1"<<endl; // :TODO: error handling
        m_io_error_state = 1;
        return;
    }

    if( !m_file.is_open() ) {
        cout<<"err2"<<endl; // :TODO: error handling
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
    if( m_mode == std::ios::out ) {
        m_file << "HepMC::IO_GenEvent-END_EVENT_LISTING" << endl << endl;
    }
    m_file.close();
    m_io_error_state = 1;
}

} // namespace HepMC3
