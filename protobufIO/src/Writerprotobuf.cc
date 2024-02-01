// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file Writerprotobuf.cc
 *  @brief Implementation of \b class Writerprotobuf
 *
 */
#include "HepMC3/Writerprotobuf.h"

#include "HepMC3/protobufUtils.h"

#include "HepMC3/Version.h"

// protobuf header files
#include "HepMC3.pb.h"

namespace HepMC3 {

std::string const ProtobufMagicHeader = "hmpb";

HEPMC3_DECLARE_WRITER_FILE(Writerprotobuf)
HEPMC3_DECLARE_WRITER_STREAM(Writerprotobuf)
HEPMC3_DECLARE_WRITER_SPSTREAM(Writerprotobuf)

/// @brief Constant
static size_t const MDBytesLength = 10;

/// @brief Write a message
size_t write_message(std::ostream *out_stream, std::string const &msg_str,
                     HepMC3_pb::MessageDigest::MessageType type) {

  HepMC3_pb::MessageDigest md;
  md.set_bytes(msg_str.size());
  md.set_message_type(type);

  std::string md_str;
  md.SerializeToString(&md_str);

  if (md_str.size() != MDBytesLength) {
    HEPMC3_ERROR("When writing protobuf message, the message digest was not "
                 "the expected length ("
                 << MDBytesLength << " bytes), but was instead "
                 << md_str.size() << " bytes.");
  }

  (*out_stream) << md_str;
  (*out_stream) << msg_str;
  return md_str.size() + msg_str.size();
}

Writerprotobuf::Writerprotobuf(const std::string &filename,
                               std::shared_ptr<GenRunInfo> run)
    : m_out_file(nullptr), m_events_written(0), m_event_bytes_written(0) {

  if (!run) {
    run = std::make_shared<GenRunInfo>();
  }
  set_run_info(run);

  // open file
  m_out_file = std::unique_ptr<ofstream>(
      new ofstream(filename, ios::out | ios::trunc | ios::binary));

  // check that it is open
  if (!m_out_file->is_open()) {
    HEPMC3_ERROR("Writerprotobuf: problem opening file: " << filename)
    return;
  }

  m_out_stream = m_out_file.get();
  start_file();
}

Writerprotobuf::Writerprotobuf(std::ostream &stream,
                               std::shared_ptr<GenRunInfo> run)
    : m_out_file(nullptr), m_events_written(0), m_event_bytes_written(0) {

  if (!stream.good()) {
    HEPMC3_ERROR(
        "Cannot initialize Writerprotobuf on ostream which is not good().");
    return;
  }

  if (!run) {
    run = std::make_shared<GenRunInfo>();
  }
  set_run_info(run);

  m_out_stream = &stream;
  start_file();
}

Writerprotobuf::Writerprotobuf(std::shared_ptr<std::ostream> stream,
                               std::shared_ptr<GenRunInfo> run)
    : Writerprotobuf(*stream, run) {
  m_shared_stream = stream;
}

void Writerprotobuf::start_file() {
  // The first 16 bytes of a HepMC protobuf file
  (*m_out_stream) << ProtobufMagicHeader;

  HepMC3_pb::Header hdr;

  (*hdr.mutable_version_str()) = HepMC3::version();
  hdr.set_version_maj((HEPMC3_VERSION_CODE / 1000000) % 1000);
  hdr.set_version_min((HEPMC3_VERSION_CODE / 1000) % 1000);
  hdr.set_version_patch(HEPMC3_VERSION_CODE % 1000);

  hdr.set_protobuf_version_maj((GOOGLE_PROTOBUF_VERSION / 1000000) % 1000);
  hdr.set_protobuf_version_min((GOOGLE_PROTOBUF_VERSION / 1000) % 1000);
  hdr.set_protobuf_version_patch(GOOGLE_PROTOBUF_VERSION % 1000);

  std::string hdr_msg;
  hdr.SerializeToString(&hdr_msg);

  write_message(m_out_stream, hdr_msg, HepMC3_pb::MessageDigest::Header);

  write_run_info();
}

void Writerprotobuf::write_event(const GenEvent &evt) {
  m_event_bytes_written += write_message(m_out_stream, Serialize::GenEvent(evt),
                                         HepMC3_pb::MessageDigest::Event);
  m_events_written++;
}

void Writerprotobuf::write_run_info() {
  write_message(m_out_stream, Serialize::GenRunInfo(*run_info()),
                HepMC3_pb::MessageDigest::RunInfo);
}

void Writerprotobuf::close() {
  if (failed()) {
    return;
  }

  if (!m_events_written) {
    HEPMC3_ERROR(
        "No events were written, the output file will not be parseable.");
  }

  HepMC3_pb::Footer ftr;
  ftr.set_nevents(m_events_written);
  ftr.set_event_bytes_written(m_event_bytes_written);
  std::string ftr_msg;
  ftr.SerializeToString(&ftr_msg);

  write_message(m_out_stream, ftr_msg, HepMC3_pb::MessageDigest::Footer);

  if (m_out_file) {
    m_out_file->close();
    m_out_file.reset();
  }
  m_out_stream = nullptr;
}

bool Writerprotobuf::failed() {
  if (m_out_file) {
    return !m_out_file->is_open() || !m_out_file->good();
  }
  return !m_out_stream || !m_out_stream->good();
}

} // namespace HepMC3
