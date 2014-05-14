#ifndef  HEPMC3_IO_FILE_BASE_H
#define  HEPMC3_IO_FILE_BASE_H
/**
 *  @file  IO_FileBase.h
 *  @brief Definition of \b class IO_FileBase
 *
 *  @class HepMC3::IO_FileBase
 *  @brief Base class for all file I/O classes
 *
 *  Serves as a file handler leaving the actual
 *  I/O operations to specific implementations
 *
 *  @ingroup IO
 */
#include <string>
#include <fstream>
#include "HepMC3/IO/IO_Base.h"
namespace HepMC3 {

class GenEvent;

class IO_FileBase : public IO_Base {
//
// Constructors
//
public:
    /** @brief Default constructor
     *  @warning Only ios::in or ios::out mode is allowed
     */
    IO_FileBase(const std::string &filename, std::ios::openmode mode);
    /** @brief Default destructor
     *
     *  Closes I/O stream (if opened)
     */
    virtual ~IO_FileBase();

//
// Functions
//
public:
    /** @brief Write event to file */
    virtual void write_event(const GenEvent &evt) = 0;

    /** @brief Get event from file */
    virtual bool fill_next_event(GenEvent &evt)   = 0;

    /** @brief Close the I/O stream */
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
