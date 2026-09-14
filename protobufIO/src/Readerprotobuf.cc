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

#include <fcntl.h>
#include <sys/stat.h>

#include <cstring>

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

Readerprotobuf::Readerprotobuf(const std::string &filename) {

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
        HEPMC3_ERROR_LEVEL(100,"Readerprotobuf: Problem opening file: " << filename)
        perror(filename.c_str());
        return;
    }

    m_inf_zcstream = std::unique_ptr<google::protobuf::io::FileInputStream>(
                         new google::protobuf::io::FileInputStream(fd));
    // We don't need to worry about closing fd
    m_inf_zcstream->SetCloseOnDelete(true);

    if (m_inf_zcstream->GetErrno()) {
        HEPMC3_ERROR_LEVEL(100,"Readerprotobuf: Problem opening file: "<< filename << "\n"<< strerror(m_inf_zcstream->GetErrno()))
        m_inf_zcstream.reset();
        return;
    }

    m_in_zcstream = m_inf_zcstream.get();

    read_file_start();
}

Readerprotobuf::Readerprotobuf(std::istream &stream) : m_in_stream(&stream) {

    if (!stream.good()) {
        HEPMC3_ERROR_LEVEL(100,"Cannot initialize Readerprotobuf on istream which is not good().")
        return;
    }

    m_in_zcistream = std::unique_ptr<google::protobuf::io::IstreamInputStream>(
                         new google::protobuf::io::IstreamInputStream(&stream));
    m_in_zcstream = m_in_zcistream.get();

    read_file_start();
}

Readerprotobuf::Readerprotobuf(std::shared_ptr<std::istream> stream)
    : Readerprotobuf(*stream) {
    m_shared_stream = stream;
}

bool Readerprotobuf::read_file_start() {

    const void *MagicIntro = nullptr;
    int nbytes = 0;
    m_in_zcstream->Next(&MagicIntro, &nbytes);

    // Check the first four bytes it should read "hmpb"
    if (strncmp(static_cast<const char *>(MagicIntro),
                ProtobufMagicHeader.c_str(), ProtobufMagicHeaderBytes) != 0) {
        HEPMC3_ERROR_LEVEL(100,"Failed to find expected Magic first "<< ProtobufMagicHeaderBytes<< " bytes, is this really a hmpb file?")
        return false;
    }

    // needed to read ProtobufMagicHeaderBytes, but read nbytes, back up
    m_in_zcstream->BackUp(nbytes - ProtobufMagicHeaderBytes);

    if (!read_Header()) {
        HEPMC3_ERROR_LEVEL(100,"Readerprotobuf: Problem parsing start of file, expected to find Header, but instead found message type: "<< m_md_pb.message_type())
        return false;
    }

    if (!read_GenRunInfo()) {
        HEPMC3_ERROR_LEVEL(100,"Readerprotobuf: Problem parsing start of file, expected to find RunInfo, but instead found message type: "<< m_md_pb.message_type())
        return false;
    }

    return true;
}

bool Readerprotobuf::read_digest() {
    if (failed()) {
        return false;
    }

    if (!m_md_pb.ParseFromBoundedZeroCopyStream(m_in_zcstream, MDBytesLength)) {
        return false;
    }

    if (failed()) {
        return false;
    }

    // close the stream if we have read to the end of the file
    if (m_md_pb.message_type() == HepMC3_pb::MessageDigest::Footer) {
        close();
    }

    return true;
}

bool Readerprotobuf::read_Header() {
    if (!read_digest()) {
        return false;
    }

    if (m_md_pb.message_type() != HepMC3_pb::MessageDigest::Header) {
        return false;
    }

    if (!m_hdr_pb.ParseFromBoundedZeroCopyStream(m_in_zcstream,
            m_md_pb.bytes())) {
        // if we fail to read a message then close the stream to indicate failed
        // state
        close();
        return false;
    }

    return true;
}

bool Readerprotobuf::read_GenRunInfo() {
    if (!read_digest()) {
        return false;
    }

    if (m_md_pb.message_type() != HepMC3_pb::MessageDigest::RunInfo) {
        return false;
    }

    set_run_info(std::make_shared<HepMC3::GenRunInfo>());

    if (!m_gri_pb.ParseFromBoundedZeroCopyStream(m_in_zcstream,
            m_md_pb.bytes())) {
        close();
        return false;
    }

    Deserialize::FillGenRunInfo(m_gri_pb, run_info());

    return true;
}

bool Readerprotobuf::read_GenEvent(bool /*skip*/, GenEvent &evt) {
    if (!read_digest()) {
        return false;
    }

    if (m_md_pb.message_type() != HepMC3_pb::MessageDigest::Event) {
        return false;
    }

    if (!m_evt_pb.ParseFromBoundedZeroCopyStream(m_in_zcstream,
            m_md_pb.bytes())) {
        close();
        return false;
    }

    evt.read_data(m_evt_pb);

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
    if (m_inf_zcstream) {
        m_inf_zcstream.reset();
    }
    if (m_in_zcistream) {
        m_in_zcistream.reset();
    }
    m_in_stream = nullptr;
}

bool Readerprotobuf::failed() {
    if (m_inf_zcstream) {
        return m_inf_zcstream->GetErrno();
    }
    return !m_in_stream;
}

} // namespace HepMC3
