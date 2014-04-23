#ifndef  HEPMC3_IO_BASE_H
#define  HEPMC3_IO_BASE_H
/**
 *  @file  IO_Base.h
 *  @brief Definition of \b class HepMC3::IO_Base
 *
 *  @class HepMC3::IO_Base
 *  @brief Abstract class serving as a base of I/O interfaces
 *
 *  Serves as a file handler leaving the actual
 *  I/O operations to implementations
 *
 *  @ingroup IO
 *
 */
#include <string>
#include <fstream>
#include "HepMC3/GenEvent.h"
namespace HepMC3 {

class IO_Base {
//
// Constructors
//
public:
    /** Default constructor
     *  @warning Only ios::in or ios::out mode is allowed
     */
    IO_Base(const std::string &filename, std::ios::openmode mode);
    /** Default destructor.
     *  Closes I/O stream (if opened)
     */
    virtual ~IO_Base();
//
// Functions
//
public:
    /** Write event to file */
    virtual void write_event(const GenEvent &evt) = 0;

    /** Get event from file */
    virtual bool fill_next_event(GenEvent &evt)   = 0;

    /** Close the I/O stream */
    void close();

//
// Accessors
//
public:

    void clear (std::ios::iostate state = std::ios::goodbit) { m_file.clear(state); }     //!< Set new value of stream's internal error state flags
    std::ios::iostate rdstate()                              { return m_file.rdstate(); } //!< Get stream's error state flags

//
// Fields
//
protected:
    std::fstream        m_file; //!< I/O stream
    std::ios::openmode  m_mode; //!< I/O stream mode
};

} // namespace HepMC3

#endif
