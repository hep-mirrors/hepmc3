#include <iostream>
#include <fstream>
#include "HepMC3/IO_GenEvent.h"
#include "HepMC3/GenEvent.h"
using std::ostream;
using std::cout;
using std::endl;

namespace HepMC3 {

void IO_GenEvent::write_event(const GenEvent *evt) {
    if ( !evt || m_io_error_state ) return;
	if ( m_mode != std::ios::out ) {
	    cout << "err3" << endl; // :TODO: error handling
	    return;
	}
    cout << "Write OK." << endl;
}

bool IO_GenEvent::fill_next_event(GenEvent *evt) {
    if ( !evt || m_io_error_state ) return 0;
	if ( m_mode != std::ios::in ) {
	    cout << "err4" << endl; // :TODO: error handling
	    return 0;
	}
    cout << "Read OK." << endl;

    return 1;
}

} // namespace HepMC3
