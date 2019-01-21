// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenCrossSection_fwd.h
/// @brief Minimal forward declarations for GenCrossSection
///

#ifndef HEPMC_GENCROSSSECTION_FWD_H
#define HEPMC_GENCROSSSECTION_FWD_H

#include <memory>

namespace HepMC{

class GenCrossSection;

using GenCrossSectionPtr = std::shared_ptr<GenCrossSection>;
using ConstGenCrossSectionnPtr = std::shared_ptr<const GenCrossSection>;

}

#endif
