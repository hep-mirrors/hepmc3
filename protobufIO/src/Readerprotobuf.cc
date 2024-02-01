// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file Readerprotobuf.cc
 *  @brief Implementation of \b class Readerprotobuf
 *
 */
#include "HepMC3/Readerprotobuf.h"

#include "HepMC3/Print.h"
#include "HepMC3/Version.h"

#include "HepMC3/protobufUtils.h"

// protobuf header files
#include "HepMC3.pb.h"

namespace HepMC3 {

/// @brief Header of the protobuf file
std::string const ProtobufMagicHeader = "hmpb";
/// @brief Size of the protobuf file header
size_t const ProtobufMagicHeaderBytes = 4;

HEPMC3_DECLARE_READER_FILE(Readerprotobuf)
HEPMC3_DECLARE_READER_STREAM(Readerprotobuf)
HEPMC3_DECLARE_READER_SPSTREAM(Readerprotobuf)

/// @brief Constant
static size_t const MDBytesLength = 10;

Readerprotobuf::Readerprotobuf(const std::string &filename)
    : m_msg_type(HepMC3_pb::MessageDigest::unknown) {

  m_md_buffer.resize(MDBytesLength);

  m_in_file = std::unique_ptr<std::ifstream>(
      new std::ifstream(filename, ios::in | ios::binary));

  if (!m_in_file->is_open()) {
    HEPMC3_ERROR("Readerprotobuf: Problem opening file: " << filename)
    return;
  }

  m_in_stream = m_in_file.get();

  read_file_start();
}

Readerprotobuf::Readerprotobuf(std::istream &stream)
    : m_msg_type(HepMC3_pb::MessageDigest::unknown) {

  if (!stream.good()) {
    HEPMC3_ERROR(
        "Cannot initialize Readerprotobuf on istream which is not good().");
    return;
  }

  m_md_buffer.resize(MDBytesLength);

  m_in_stream = &stream;
  read_file_start();
}

Readerprotobuf::Readerprotobuf(std::shared_ptr<std::istream> stream)
    : Readerprotobuf(*stream) {
  m_shared_stream = stream;
}

bool Readerprotobuf::read_file_start() {

  // Read the first 4 bytes, it should read "hmpb"
  std::string MagicIntro;
  MagicIntro.resize(ProtobufMagicHeaderBytes);
  m_in_stream->read(&MagicIntro[0], ProtobufMagicHeaderBytes);

  if (MagicIntro != ProtobufMagicHeader) {
    HEPMC3_ERROR("Failed to find expected Magic first "
                 << ProtobufMagicHeaderBytes
                 << " bytes, is this really "
                    "a hmpb file?");
    return false;
  }

  if (!read_Header()) {
    HEPMC3_ERROR("Readerprotobuf: Problem parsing start of file, expected to "
                 "find Header, but instead found message type: "
                 << m_msg_type);
    return false;
  }

  if (!read_GenRunInfo()) {
    HEPMC3_ERROR("Readerprotobuf: Problem parsing start of file, expected to "
                 "find RunInfo, but instead found message type: "
                 << m_msg_type);
    return false;
  }

  return true;
}

bool Readerprotobuf::buffer_message() {
  if (failed()) {
    return false;
  }

  if (m_msg_buffer.size()) { // if we already have a message that hasn't been
    // parsed, don't buffer the next one
    return true;
  }

  m_msg_type = HepMC3_pb::MessageDigest::unknown;

  m_in_stream->read(&m_md_buffer[0], MDBytesLength);

  if (failed()) {
    return false;
  }

  m_bytes_read += MDBytesLength;

  HepMC3_pb::MessageDigest md;
  if (!md.ParseFromString(m_md_buffer)) {
    return false;
  }

  m_msg_type = md.message_type();

  m_msg_buffer.resize(md.bytes());
  m_in_stream->read(&m_msg_buffer[0], md.bytes());

  if (failed()) {
    return false;
  }

  m_bytes_read += md.bytes();

  if (m_msg_type ==
      HepMC3_pb::MessageDigest::Footer) { // close the stream if we have read to
    // the end of the file
    close();
  }

  return true;
}

bool Readerprotobuf::read_Header() {
  if (!buffer_message()) {
    return false;
  }

  if (m_msg_type != HepMC3_pb::MessageDigest::Header) {
    return false;
  }

  HepMC3_pb::Header Header_pb;
  if (!Header_pb.ParseFromString(m_msg_buffer)) {
    // if we fail to read a message then close the stream to indicate failed
    // state
    close();
    return false;
  }
  m_msg_buffer.clear();

  m_file_header.m_version_str = Header_pb.version_str();
  m_file_header.m_version_maj = Header_pb.version_maj();
  m_file_header.m_version_min = Header_pb.version_min();
  m_file_header.m_version_patch = Header_pb.version_patch();
  m_file_header.m_protobuf_version_maj = Header_pb.protobuf_version_maj();
  m_file_header.m_protobuf_version_min = Header_pb.protobuf_version_min();
  m_file_header.m_protobuf_version_patch = Header_pb.protobuf_version_patch();

  return true;
}

bool Readerprotobuf::read_GenRunInfo() {
  if (!buffer_message()) {
    return false;
  }

  if (m_msg_type != HepMC3_pb::MessageDigest::RunInfo) {
    return false;
  }

  set_run_info(std::make_shared<HepMC3::GenRunInfo>());

  if (!Deserialize::GenRunInfo(m_msg_buffer, run_info())) {
    // if we fail to read a message then close the stream to indicate failed
    // state
    close();
    return false;
  }

  m_msg_buffer.clear();
  return true;
}

bool Readerprotobuf::read_GenEvent(bool skip, GenEvent &evt) {
  if (!buffer_message()) {
    return false;
  }

  if (m_msg_type != HepMC3_pb::MessageDigest::Event) {
    return false;
  }

  if (skip) { // Don't parse to HepMC3 if skipping
    m_msg_buffer.clear();
    return true;
  }

  if (!m_msg_buffer.size()) { // empty event
    return true;
  }

  if (!Deserialize::GenEvent(m_msg_buffer, evt)) {
    // if we fail to read a message then close the stream to indicate failed
    // state
    close();
    return false;
  }

  m_msg_buffer.clear();
  return true;
}

bool Readerprotobuf::skip(const int n) {
  static HepMC3::GenEvent dummyev;
  for (int nn = n; nn > 0; --nn) {
    if (!read_GenEvent(true, dummyev)) {
      return false;
    }
  }
  return !failed();
}

bool Readerprotobuf::read_event(GenEvent &evt) {

  if (!read_GenEvent(false, evt)) {
    return false;
  }

  evt.set_run_info(run_info());

  return true;
}

void Readerprotobuf::close() {
  if (m_in_file) {
    m_in_file->close();
    m_in_file.reset();
  }
  m_in_stream = nullptr;
  m_msg_buffer.clear();
}

bool Readerprotobuf::failed() {
  if (m_in_file) {
    return !m_in_file->is_open() || !m_in_file->good();
  }
  return !m_in_stream || !m_in_stream->good();
}

} // namespace HepMC3
