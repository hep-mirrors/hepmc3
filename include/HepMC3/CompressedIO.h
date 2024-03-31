// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_COMPRESSEDIO_H
#define HEPMC3_COMPRESSEDIO_H
#if HEPMC3_USE_COMPRESSION
#if HEPMC3_Z_SUPPORT
#define BXZSTR_Z_SUPPORT 1
#endif
#if HEPMC3_LZMA_SUPPORT
#define BXZSTR_LZMA_SUPPORT 1
#endif
#if HEPMC3_BZ2_SUPPORT
#define BXZSTR_BZ2_SUPPORT 1
#endif
#if HEPMC3_ZSTD_SUPPORT
#define BXZSTR_ZSTD_SUPPORT 1
#endif
#endif
#include "HepMC3/bxzstr/bxzstr.hpp"

#include <array>

namespace HepMC3
{
using ofstream = bxz::ofstream;
using ostream = bxz::ostream;
using ifstream = bxz::ifstream;
using istream = bxz::istream;

using Compression = bxz::Compression;
inline Compression detect_compression_type(const char* in_buff_start, const char* in_buff_end) {
    return bxz::detect_type(in_buff_start,in_buff_end);
}
constexpr int num_supported_compression_types = 0
#if HEPMC3_Z_SUPPORT
        +1
#endif
#if HEPMC3_LZMA_SUPPORT
        +1
#endif
#if HEPMC3_BZ2_SUPPORT
        +1
#endif
#if HEPMC3_ZSTD_SUPPORT
        +1
#endif
        ;
constexpr std::array<Compression,num_supported_compression_types> supported_compression_types{
#if HEPMC3_Z_SUPPORT
    Compression::z,
#endif
#if HEPMC3_LZMA_SUPPORT
    Compression::lzma,
#endif
#if HEPMC3_BZ2_SUPPORT
    Compression::bz2,
#endif
#if HEPMC3_ZSTD_SUPPORT
    Compression::zstd,
#endif
};
constexpr std::array<Compression, 4> known_compression_types{
    Compression::z,
    Compression::lzma,
    Compression::bz2,
    Compression::zstd,
};

inline std::string to_string(HepMC3::Compression & c) {
    switch (c) {
    case HepMC3::Compression::z:
        return string("z");
    case HepMC3::Compression::lzma:
        return string("lzma");
    case HepMC3::Compression::bz2:
        return string("bz2");
    case HepMC3::Compression::zstd:
        return string("zstd");
    default:
        break;
    }
    return string("plaintext");
}

}

inline std::ostream& operator<<(std::ostream& os, HepMC3::Compression & c) {
    return os << HepMC3::to_string(c);
}


#endif
