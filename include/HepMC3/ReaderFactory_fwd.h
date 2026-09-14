// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERFACTORY_FWD_H
#define HEPMC3_READERFACTORY_FWD_H

#include <memory>
#include <string>
#include <sys/stat.h>
#include <string.h>

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#include "HepMC3/ReaderOSCAR1997.h"
#include "HepMC3/ReaderOSCAR1999.h"
#include "HepMC3/ReaderOSCAR2013.h"
#include "HepMC3/ReaderPlugin.h"
#include "HepMC3/CompressedIO.h"

namespace HepMC3 {
#if ! (defined(__darwin__) || defined(__APPLE__)) &&  ! ((defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__))
const std::string libHepMC3rootIO = "libHepMC3rootIO.so.3";
const std::string libHepMC3protobufIO = "libHepMC3protobufIO.so.1";
#endif
#if defined(__darwin__) || defined(__APPLE__)
const std::string libHepMC3rootIO = "libHepMC3rootIO.dylib";
const std::string libHepMC3protobufIO = "libHepMC3protobufIO.dylib";
#endif
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__)
const std::string libHepMC3protobufIO = "HepMC3protobufIO.dll";
const std::string libHepMC3rootIO = "HepMC3rootIO.dll";
#endif
/** Information about input.

    This class deduces information about the input file from the
    filename, such as

    - is it a remote file
    - is reading from it an error
    - is it a ROOT file
    - is it a protobuf file
    - is it ASCII (v3)
    - is it ASCII (v2)
    - is it LHEF file
    - is it HEPEVT

    It can also return a reader
*/
class InputInfo {
public:
    /// @brief Constructor
    InputInfo() {};
    /// @brief Constructor
    InputInfo(const std::string &filename);
    /// @brief Classify input
    void classify();
    std::vector<std::string> m_head; ///< Non-empty lines from the input header.
    bool m_remote = false; ///< Whether the input is addressed by a remote URL.
    bool m_pipe = false; ///< Whether the input is a named pipe.
    bool m_error = false; ///< Whether input inspection encountered an error.
    bool m_init = false; ///< Whether input inspection completed.
    bool m_root = false; ///< Whether the input is a ROOT file.
    bool m_protobuf = false; ///< Whether the input is a protobuf file.
    bool m_asciiv3 = false; ///< Whether the input uses HepMC3 ASCII format.
    bool m_iogenevent = false; ///< Whether the input uses HepMC2 IO_GenEvent format.
    bool m_lhef = false; ///< Whether the input uses LHEF format.
    bool m_hepevt = false; ///< Whether the input uses HEPEVT format.
    bool m_OSCAR2013 = false;
    bool m_OSCAR1999 = false;
    bool m_OSCAR1997 = false;
    std::shared_ptr<Reader> m_reader = nullptr; ///< Native reader selected during inspection.
    /// @brief Get native (built-in) reader
    template <class T> std::shared_ptr<Reader> native_reader(T& argument);
};

std::shared_ptr<Reader> deduce_reader(std::istream &stream);

std::shared_ptr<Reader> deduce_reader(std::shared_ptr<std::istream> stream);

template <class T> std::shared_ptr<Reader> InputInfo::native_reader(T& argument) {

    if (m_asciiv3) {
        HEPMC3_DEBUG(10, "Attempt ReaderAscii");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderAscii(argument)));
    }
    if (m_iogenevent) {
        HEPMC3_DEBUG(10, "Attempt ReaderAsciiHepMC2");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderAsciiHepMC2(argument)));
    }
    if (m_lhef) {
        HEPMC3_DEBUG(10, "Attempt ReaderLHEF");

        return std::shared_ptr<Reader>((Reader*) ( new ReaderLHEF(argument)));
    }
    if (m_hepevt)  {
        HEPMC3_DEBUG(10, "Attempt ReaderHEPEVT");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderHEPEVT(argument)));
    }
    if (m_OSCAR2013)  {
        HEPMC3_DEBUG(10, "Attempt ReaderOSCAR2013");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderOSCAR2013(argument)));
    }
    if (m_OSCAR1999)  {
        HEPMC3_DEBUG(10, "Attempt ReaderOSCAR1999");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderOSCAR1999(argument)));
    }
    if (m_OSCAR1997)  {
        HEPMC3_DEBUG(10, "Attempt ReaderOSCAR1997");
        return std::shared_ptr<Reader>((Reader*) ( new ReaderOSCAR1997(argument)));
    }
    HEPMC3_DEBUG(10, "deduce_reader: all attempts failed");
    return std::shared_ptr<Reader>(nullptr);
}

}
#endif
