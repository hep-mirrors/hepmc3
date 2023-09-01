// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderHDF5.cc
 *  @brief Implementation of \b class ReaderHDF5
 *
 */
#include "HepMC3/Version.h"
#include "HepMC3/IOHDF5.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/Data/GenRunInfoData.h"

#include <highfive/H5File.hpp>
using HighFive::File;

HighFive::CompoundType create_compound_GenParticleData() {
    return { {"pid", HighFive::AtomicType<int>{}}
        ,{"status", HighFive::AtomicType<int>{}}
        ,{"is_mass_set", HighFive::AtomicType<bool>{}}
        ,{"mass", HighFive::AtomicType<double>{}}
        ,{"momentum_0", HighFive::AtomicType<double>{}}
        ,{"momentum_1", HighFive::AtomicType<double>{}}
        ,{"momentum_2", HighFive::AtomicType<double>{}}
        ,{"momentum_3", HighFive::AtomicType<double>{}}
    };
}
HighFive::CompoundType create_compound_GenVertexData() {
    return { {"status", HighFive::AtomicType<int>{}}
        ,{"position_0", HighFive::AtomicType<double>{}}
        ,{"position_1", HighFive::AtomicType<double>{}}
        ,{"position_2", HighFive::AtomicType<double>{}}
        ,{"position_3", HighFive::AtomicType<double>{}}
    };
}



using HighFive::File;

#define HIGHFIVE_REGISTER_TYPE_NOINLINE(type, function)                    \
    template <>                                                   \
     HighFive::DataType HighFive::create_datatype<type>() { \
        return function();                                        \
    }
HIGHFIVE_REGISTER_TYPE_NOINLINE(HepMC3::GenParticleData, create_compound_GenParticleData)
HIGHFIVE_REGISTER_TYPE_NOINLINE(HepMC3::GenVertexData, create_compound_GenVertexData)
