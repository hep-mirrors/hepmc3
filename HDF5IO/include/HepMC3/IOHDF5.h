// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_IOHDF5_H
#define HEPMC3_IOHDF5_H
#include <highfive/H5File.hpp>
#include <highfive/H5DataType.hpp>
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/Data/GenRunInfoData.h"

using HighFive::File;
HighFive::CompoundType create_compound_GenParticleData();
HighFive::CompoundType create_compound_GenVertexData();
HighFive::CompoundType create_compound_GenRunInfoData();
template <> HighFive::DataType HighFive::create_datatype<HepMC3::GenParticleData>();
template <> HighFive::DataType HighFive::create_datatype<HepMC3::GenVertexData>();
template <> HighFive::DataType HighFive::create_datatype<HepMC3::GenRunInfoData>();
#endif