// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_IO_GENEVENT_H
#define  HEPMC_IO_GENEVENT_H
/**
 *  @file  IO_GenEvent.h
 *  @brief Definition of \b class IO_GenEvent
 *
 *  @class HepMC::IO_GenEvent
 *  @brief GenEvent I/O parsing and serialization for human-readable text files
 *
 *  @ingroup IO
 *
 */
#include "HepMC/IO/IO_FileBase.h"

#include "HepMC/GenEvent.h"

#include <string>
#include <fstream>

namespace HepMC {

class GenEvent;

class IO_GenEvent : public IO_FileBase {
//
// Constructors
//
public:
    /** @brief Default constructor
     *
     *  Mode of operation determines if file is read-only or write-only
     *  @warning If file opened in write mode exists, it will be deleted
     */
    IO_GenEvent(const std::string &filename, std::ios::openmode mode);

    /** @brief Default destructor */
    ~IO_GenEvent();

//
// Functions
//
public:
    /** @brief Write event to file
     *
     *  @param[in] evt Event to be serialized
     */
    void write_event(const GenEvent &evt);

    /** @brief Load event from file
     *
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent &evt);

    /** @brief Attempts to allocate buffer of the chosen size
     *
     *  This function can be called manually by the user or will be called
     *  before first read/write operation
     *  @note If buffer size is to large it will be divided by 2
     *        until it is small enough for system to allocate
     */
    void allocate_buffer();

private:
    /** @brief Inline function for writing strings
     *
     *  Since strings can be long (maybe even longer than buffer) they have to be delt
     *  with separately.
     */
    void write_string( const std::string &str );

    void flush();        //!< Inline function flushing buffer to output stream when close to buffer capacity
    void forced_flush(); //!< Inline function forcing flush to the output stream

private:
    /** @brief Write vertex
     *
     *  Helper routine for writing single vertex to file
     */
    void write_vertex  (const GenVertexPtr &v);

    /** @brief Write particle
     *
     *  Helper routine for writing single particle to file
     */
    void write_particle(const GenParticlePtr &p, int second_field);

private:
    /** @brief Parse event
     *
     *  Helper routine for parsing event information
     *  @param[out] evt Event that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     *  @return vertices count and particles count for verification
     */
    std::pair<int,int> parse_event_information(GenEvent &evt, const char *buf);

    /** @brief Parse units
     *
     *  Helper routine for parsing units information
     *  @param[out] evt Event that will be filled with unit information
     *  @param[in]  buf Line of text that needs to be parsed
     */
    bool parse_units(GenEvent &evt, const char *buf);

    /** @brief Parse vertex
     *
     *  Helper routine for parsing single event information
     *  @param[out] evt Event that will contain parsed vertex
     *  @param[in] buf Line of text that needs to be parsed
     */
    bool parse_vertex_information(GenEvent &evt, const char *buf);

    /** @brief Parse particle
     *
     *  Helper routine for parsing single particle information
     *  @param[out] evt Event that will contain parsed particle
     *  @param[in] buf Line of text that needs to be parsed
     */
    bool parse_particle_information(GenEvent &evt, const char *buf);

    /** @brief Parse attribute
     *
     *  Helper routine for parsing attribute information
     *  @param[out] evt Event that will contain parsed particle
     *  @param[in] buf Line of text that needs to be parsed
     */
    bool parse_attribute(GenEvent &evt, const char *buf);
//
// Accessors
//
public:
    /** @brief Set output precision
     *
     *  Available range is [2,24]. Default is 16.
     */
    void set_precision( unsigned int prec ) {
        if( prec < 2 || prec > 24 ) return;
        m_precision = prec;
    }

    /** @brief Set buffer size (in bytes)
     *
     *  Default is 256kb. Minimum is 256b.
     *  Size can only be changed before first read/write operation.
     */
    void set_buffer_size( unsigned long size) {
        if( m_buffer )   return;
        if( size < 256 ) return;

        m_buffer_size = size;
    }

//
// Fields
//
private:
    int            m_precision;   //!< Output precision
    char          *m_buffer;      //!< Stream buffer
    char          *m_cursor;      //!< Cursor inside stream buffer
    unsigned long  m_buffer_size; //!< Buffer size
};

} // namespace HepMC

#endif
