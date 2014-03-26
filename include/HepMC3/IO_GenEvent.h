#ifndef  HEPMC3_IO_GENEVENT_H
#define  HEPMC3_IO_GENEVENT_H
/**
 *  @file  IO_GenEvent.h
 *  @brief Definition of \b class HepMC3::IO_GenEvent
 *
 *  @class HepMC3::IO_GenEvent
 *  @brief GenEvent I/O parsing and serialization for human-readable text files
 *
 *  @date Created       <b> 23th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
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
    /** Default constructor
     *  Mode of operation determines if file is read-only or write-only
     *  @warning If file opened in write-only mode exists, it will be deleted
     */
    IO_GenEvent(const std::string &filename, std::ios::openmode mode):IO_Base(filename,mode),m_precision(16) {};

//
// Functions
//
public:
    /** Write event to file
     *  @param[in] evt Event to be serialized
     */
    void write_event(const GenEvent *evt);

    /** Load event from file
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent *evt);
    
    /** Set output precision
     *  Default = 16
     */
    void set_precision(int prec)  { m_precision = prec; }

private:
    /** Write vertex
     *  Helper routine for writing single vertex to file
     */
    void write_vertex  (const GenVertex *v);

    /** Write particle
     *  Helper routine for writing single particle to file
     */
    void write_particle(const GenParticle *p);

//
// Fields
//
private:
    int m_precision; //!< Output precision
};

} // namespace HepMC3

#endif
