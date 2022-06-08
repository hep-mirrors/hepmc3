// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2022 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERPROTOBUF_H
#define HEPMC3_READERPROTOBUF_H
/**
 *  @file  Readerprotobuf.h
 *  @brief Definition of \b class Readerprotobuf
 *
 *  @class HepMC3::Readerprotobuf
 *  @brief GenEvent I/O parsing and serialization for protobuf-based binary
 * files
 *
 * If HepMC was compiled with path to protobuf available, this class can be
 * used for protobuf file I/O in the same manner as with HepMC::ReaderAscii
 * class.
 *
 *  @ingroup IO
 *
 */

#include "HepMC3/Reader.h"

#include "HepMC3/GenEvent.h"

#include "HepMC3/Data/GenEventData.h"

#include <array>
#include <fstream>
#include <string>
#include <vector>

namespace HepMC3 {

class Readerprotobuf : public Reader {
public:
  /**
   * @class HepMC3::Readerprotobuf::FileHeader
   * @brief A copy of the information contained in the protobuf file header
   */
  struct FileHeader {
    std::string m_version_str;
    unsigned int m_version_maj;
    unsigned int m_version_min;
    unsigned int m_version_patch;

    unsigned int m_protobuf_version_maj;
    unsigned int m_protobuf_version_min;
    unsigned int m_protobuf_version_patch;
  };

  //
  // Constructors
  //
public:
  /** @brief filename constructor
   *
   * @details Attempts to open the passed filename and read protobuf HepMC3
   * events from it
   */
  Readerprotobuf(const std::string &filename);

  /** @brief istream constructor
   *
   * @details Attempts to read a binary HepMC3 protobuf event stream from the
   * passed istream object
   */
  Readerprotobuf(std::istream &stream);

  /** @brief istream constructor
   *
   * @details Attempts to read a binary HepMC3 protobuf event stream from the
   * passed istream object
   */
  Readerprotobuf(std::shared_ptr<std::istream> stream);

  //
  // Functions
  //
public:
  /** @brief skips the next N events
   *
   *  @param[in] the number of events to skip
   *  @return Whether the reader can still be read from after skipping
   */
  bool skip(const int) override;

  /** @brief Read event from file
   *
   *  @param[out] evt Contains parsed event
   *  @return Whether the reader can still be read from after reading
   */
  bool read_event(GenEvent &evt) override;

  /** @brief Close file stream */
  void close() override;

  /** @brief Get the header information read from the protobuf file */
  FileHeader const &file_header() { return m_file_header; }

  /** @brief Get stream error state */
  bool failed() override;
  //
  // Fields
  //
private:
  /** @brief Read the next protobuf message into the message buffer
   *
   * @details Fills m_msg_buffer with the next message and sets m_msg_type to
   * signify the message type. Returns true if there is a message in the buffer
   * ready to parse.
   */
  bool buffer_message();

  /** @brief Parse the next protobuf message as a GenRunInfo message
   *
   * @return Whether the reader can still be read from after reading
   */
  bool read_GenRunInfo();

  /** @brief Parse the next protobuf message as a GenEvent message
   *
   * @param[in] skip Whether to bother actually parsing this message to a
   * GenEvent
   * @return Whether the reader can still be read from after reading
   */
  bool read_GenEvent(bool skip = false);

  /** @brief Parse the next protobuf message as a Header message
   *
   * @return Whether the reader can still be read from after reading
   */
  bool read_Header();

  /** @brief Parse the front matter of the protobuf message stream before the
   * events
   */
  bool read_file_start();

  size_t m_bytes_read; //!< Document me

  std::unique_ptr<std::ifstream> m_in_file;//!< Document me
  std::istream *m_in_stream;//!< Document me

  std::string m_msg_buffer;//!< Document me
  std::string m_md_buffer;//!< Document me
  int m_msg_type;//!< Document me

  HepMC3::GenEventData m_evdata;//!< Document me

  FileHeader m_file_header;//!< Document me
};

} // namespace HepMC3

#endif
