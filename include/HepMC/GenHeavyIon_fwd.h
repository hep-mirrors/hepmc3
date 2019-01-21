// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2018 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenHeavyIon_fwd.h
/// @brief Minimal forward declarations for GenHeavyIon
///

#ifndef HEPMC_GENHEAVYION_FWD_H
#define HEPMC_GENHEAVYION_FWD_H

#include <memory>

namespace HepMC{

class GenHeavyIon;

using GenHeavyIonPtr = std::shared_ptr<GenHeavyIon>;
using ConstGenHeavyIonPtr = std::shared_ptr<const GenHeavyIon>;

}

#endif