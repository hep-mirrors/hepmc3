#ifndef  HEPMC3_IO_GENEVENT_H
#define  HEPMC3_IO_GENEVENT_H
#include <string>
#include <fstream>
#include "HepMC3/IO_Base.h"

namespace HepMC3 {

class GenEvent;

class IO_GenEvent : public IO_Base {
//
// Constructors
//
public:
    IO_GenEvent(const std::string &filename, std::ios::openmode mode):IO_Base(filename,mode) {};

//
// Functions
//
public:
    /// Write event to file
    void write_event(const GenEvent *evt);

    /// Get event from file
    bool fill_next_event(GenEvent *evt);
};

} // namespace HepMC3

#endif
