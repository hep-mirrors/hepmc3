// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
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

// protobuf header files
#include "HepMC3/HepMC3.pb.h"

#include "google/protobuf/io/zero_copy_stream_impl.h"

#include <array>
#include <fstream>
#include <string>
#include <vector>

namespace HepMC3 {

class Readerprotobuf : public Reader {
public:

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
  bool skip(const int n) override;

  /** @brief Read event from file
   *
   *  @param[out] evt Contains parsed event
   *  @return Whether the reader can still be read from after reading
   */
  bool read_event(GenEvent &evt) override;

  /** @brief Close file stream */
  void close() override;

  /** @brief Get stream error state */
  bool failed() override;
  //
  // Fields
  //
private:
  /** @brief Read the next protobuf message digest
   *
   * @details Determines the type and byte length of the next payload
   */
  bool read_digest();

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
  bool read_GenEvent(bool skip, GenEvent &);

  /** @brief Parse the next protobuf message as a Header message
   *
   * @return Whether the reader can still be read from after reading
   */
  bool read_Header();

  /** @brief Parse the front matter of the protobuf message stream before the
   * events
   */
  bool read_file_start();

  /** @brief Passed in shared_ptr to an input stream
   *
   * @details This is non-null and shared by this class if constructed with the
   * stream constructor
   */
  std::shared_ptr<std::istream> m_shared_stream;
  /** @brief The stream object that is read from
   *
   * @details If constructed with either stream constructor this lets us check
   * we can use this to check stream status
   */
  std::istream *m_in_stream = nullptr;

  std::unique_ptr<google::protobuf::io::FileInputStream> m_inf_zcstream;
  std::unique_ptr<google::protobuf::io::IstreamInputStream> m_in_zcistream;
  google::protobuf::io::ZeroCopyInputStream *m_in_zcstream = nullptr;

  HepMC3_pb::MessageDigest m_md_pb;
  HepMC3_pb::Header m_hdr_pb;
  HepMC3_pb::GenRunInfoData m_gri_pb;
  HepMC3_pb::GenEventData m_evt_pb;
};

} // namespace HepMC3

#endif
