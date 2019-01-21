// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenPdfInfo_fwd.h
/// @brief Minimal forward declarations for GenPdfInfo
///

#ifndef HEPMC_GENPDFINFO_FWD_H
#define HEPMC_GENPDFINFO_FWD_H

#include <memory>

namespace HepMC{

class GenPdfInfo;

using GenPdfInfoPtr = std::shared_ptr<GenPdfInfo>;
using ConstGenPdfInfoPtr = std::shared_ptr<const GenPdfInfo>;

}

#endif
