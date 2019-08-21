// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Relatives.cc
/// @brief Implementation of \b Relatives class
///
#include "HepMC3/Relatives.h"

namespace HepMC3 {
const Parents     Relatives::PARENTS;
const Children    Relatives::CHILDREN;
thread_local const Ancestors   Relatives::ANCESTORS;
thread_local const Descendants Relatives::DESCENDANTS;
}

