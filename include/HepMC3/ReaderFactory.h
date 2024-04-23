// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderFactory.h
 *  @brief Declaration of \b deduce_reader and related functions
 *
 */
#ifndef HEPMC3_READERFACTORY_H
#define HEPMC3_READERFACTORY_H


#include "HepMC3/ReaderFactory_fwd.h"
#include "HepMC3/CompressedIO.h"

namespace HepMC3 {

/**
 * @brief This function deduces the type of input file based on the name/URL
 * and its content, and will return an appropriate Reader object.
 *
 */

inline std::shared_ptr<Reader> deduce_reader(const std::string &filename)
{
    InputInfo input(filename);
    if (input.m_init && !input.m_error && input.m_reader) return input.m_reader;
    if (input.m_root || input.m_remote) {
        HEPMC3_DEBUG(10, "deduce_reader: Attempt ReaderRootTree for " << filename);
        return   std::make_shared<ReaderPlugin>(filename, libHepMC3rootIO, std::string("newReaderRootTreefile"));
    }
    if (input.m_protobuf) {
        HEPMC3_DEBUG(10, "deduce_reader: Attempt ProtobufIO for " << filename);
        return std::make_shared<ReaderPlugin>(filename, libHepMC3protobufIO, std::string("newReaderprotobuffile"));
    }
#if HEPMC3_USE_COMPRESSION
    HEPMC3_DEBUG(10, "Attempt ReaderGZ for " << filename);
    char buf[6];
    snprintf(buf, 6, "%s", input.m_head.at(0).c_str());
    Compression det = detect_compression_type(buf, buf + 6);
    if ( det != Compression::plaintext ) {
        HEPMC3_DEBUG(10, "Detected supported compression: " << det);
        return deduce_reader(std::shared_ptr< std::istream >(new ifstream(filename.c_str())));
    }
#endif
    std::string f = filename;
    return input.native_reader(f);
}
}
#endif
