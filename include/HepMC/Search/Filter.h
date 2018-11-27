// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2018 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Filter.h
/// @brief Defines Filter operations for combingin Filters
///
#ifndef HEPMC_FILTER_H
#define HEPMC_FILTER_H

#include "HepMC/GenParticle.h"

namespace HepMC{
  
  using Filter = std::function<bool(ConstGenParticlePtr)>;
  
  /// @brief The logical AND of two Filters is itself a Filter
  inline Filter operator && (const Filter & lhs, const Filter &rhs){
    return [lhs, rhs](ConstGenParticlePtr p)->bool{return lhs(p) && rhs(p); };
  }
  
  /// @brief The logical OR of two Filters is itself a Filter
  inline Filter operator || (const Filter & lhs, const Filter &rhs){
    return [lhs, rhs](ConstGenParticlePtr p)->bool{return lhs(p) || rhs(p); };
  }
  
  /// @brief The negation of a Filter is itself a Filter
  inline Filter operator !(const Filter &rhs){
    return [rhs](ConstGenParticlePtr p)->bool{return ! (rhs(p));};
  }
  
}
#endif
