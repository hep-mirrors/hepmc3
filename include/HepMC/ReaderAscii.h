// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_READERASCII_H
#define HEPMC_READERASCII_H

#include "HepMC/IO/IO_FileBase.h"
#include "HepMC/GenEvent.h"
#include <string>
#include <fstream>

namespace HepMC {



  /// @brief GenEvent I/O parsing and serialization for structured text files
  /// @ingroup IO
  class IO_GenEvent : public IO_FileBase {
  public:

    /// @brief Constructor
    /// @warning If file already exists, it will be cleared before writing
    IO_GenEvent(const std::string& filename);

    /// @brief Destructor
    ~IO_GenEvent();


    /// @brief Load event from file
    ///
    /// @param[out] evt Event to be filled
    bool read_event(GenEvent& evt);


  private:

    /// @name Buffer management
    //@{

    /// @brief Attempts to allocate buffer of the chosen size
    ///
    /// This function can be called manually by the user or will be called
    /// before first read/write operation
    ///
    /// @note If buffer size is too large it will be divided by 2 until it is
    /// small enough for system to allocate
    void allocate_buffer();

    /// @brief Inline function for writing strings
    ///
    /// Since strings can be long (maybe even longer than buffer) they have to be dealt
    /// with separately.
    void write_string( const std::string &str );

    /// Inline function flushing buffer to output stream when close to buffer capacity
    void flush();
    /// Inline function forcing flush to the output stream
    void forced_flush();

    //@}


    /// @name Write helpers
    //@{

    /// @brief Write vertex
    ///
    /// Helper routine for writing single vertex to file
    void write_vertex  (const GenVertexPtr &v);

    /// @brief Write particle
    ///
    /// Helper routine for writing single particle to file
    void write_particle(const GenParticlePtr &p, int second_field);


    //@}


    /// @name Read helpers
    //@{

    /// @brief Parse event
    ///
    /// Helper routine for parsing event information
    /// @param[out] evt Event that will be filled with new data
    /// @param[in]  buf Line of text that needs to be parsed
    /// @return vertices count and particles count for verification
    std::pair<int,int> parse_event_information(GenEvent &evt, const char *buf);

    /// @brief Parse units
    ///
    /// Helper routine for parsing units information
    /// @param[out] evt Event that will be filled with unit information
    /// @param[in]  buf Line of text that needs to be parsed
    ///
    bool parse_units(GenEvent &evt, const char *buf);

    /// @brief Parse struct GenPdfInfo information
    ///
    /// Helper routine for parsing PDF information
    /// @param[out] evt Event that will be filled with unit information
    /// @param[in]  buf Line of text that needs to be parsed
    bool parse_pdf_info(GenEvent &evt, const char *buf);

    /// @brief Parse struct GenHeavyIon information
    ///
    /// Helper routine for parsing heavy ion information
    /// @param[out] evt Event that will be filled with unit information
    /// @param[in]  buf Line of text that needs to be parsed
    bool parse_heavy_ion(GenEvent &evt, const char *buf);

    /// @brief Parse struct GenCrossSection information
    ///
    /// Helper routine for parsing cross-section information
    /// @param[out] evt Event that will be filled with unit information
    /// @param[in]  buf Line of text that needs to be parsed
    bool parse_cross_section(GenEvent &evt, const char *buf);

    /// @brief Parse vertex
    ///
    /// Helper routine for parsing single event information
    /// @param[out] evt Event that will contain parsed vertex
    /// @param[in] buf Line of text that needs to be parsed
    ///
    bool parse_vertex_information(GenEvent &evt, const char *buf);

    /// @brief Parse particle
    ///
    /// Helper routine for parsing single particle information
    /// @param[out] evt Event that will contain parsed particle
    /// @param[in] buf Line of text that needs to be parsed
    bool parse_particle_information(GenEvent &evt, const char *buf);

    //@}


  public:

    /// @brief Set output precision
    ///
    /// Available range is [2,24]. Default is 16.
    ///
    /// @todo Arg should be size_t?
    void set_precision( unsigned int prec ) {
      if (prec < 2 || prec > 24) return;
      m_precision = prec;
    }

    /// @brief Set buffer size (in bytes)
    ///
    /// Default is 256kb. Minimum is 256b.
    /// Size can only be changed before first read/write operation.
    ///
    /// @todo Arg should be size_t?
    void set_buffer_size( unsigned long size) {
      if (m_buffer) return;
      if (size < 256) return;
      m_buffer_size = size;
    }


  private:

    int m_precision; //!< Output precision
    char* m_buffer;  //!< Stream buffer
    char* m_cursor;  //!< Cursor inside stream buffer
    unsigned long m_buffer_size; //!< Buffer size

  };


} // namespace HepMC

#endif