#ifndef  HEPMC3_IO_BASE_H
#define  HEPMC3_IO_BASE_H
#include <string>
#include <fstream>
#include "HepMC3/GenEvent.h"

namespace HepMC3 {

class IO_Base {
//
// Constructors
//
public:
    IO_Base(const std::string &filename, std::ios::openmode mode);
    ~IO_Base();
//
// Functions
//
public:
    /// Write event to file
    virtual void write_event(const GenEvent *evt) = 0;

    /// Get event from file
    virtual bool fill_next_event(GenEvent *evt)   = 0;

    /// Close the IO stream
    void close();

//
// Accessors
//
public:
    /// Get IO stream error state
    std::ios::iostate rdstate() { return m_file.rdstate(); }

//
// Fields
//
protected:
    std::fstream        m_file;
    std::ios::openmode  m_mode;
    int m_io_error_state;
};

} // namespace HepMC3

#endif
