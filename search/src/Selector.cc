// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Selector.cc
/// @brief Implementation  of Selector wrappers
///
#include "HepMC3/Selector.h"

namespace HepMC3 {
  const SelectorWrapper<int> Selector::STATUS      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->status();});
  const SelectorWrapper<int> Selector::PDG_ID      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->pdg_id();});
  const SelectorWrapper<double> Selector::PT       = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().pt();});
  const SelectorWrapper<double> Selector::ENERGY   = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().e();});
  const SelectorWrapper<double> Selector::RAPIDITY = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().rap();});
  const SelectorWrapper<double> Selector::ETA      = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().eta();});
  const SelectorWrapper<double> Selector::PHI      = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().phi();});
  const SelectorWrapper<double> Selector::ET       = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().e() * (p->momentum().pt() / p->momentum().p3mod());});
  const SelectorWrapper<double> Selector::MASS     = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().m();});


  
    ConstSelectorPtr abs(const Selector &input)
  {
    return input.abs();
  }

  
  AttributeFeature Selector::ATTRIBUTE(const std::string &name){return AttributeFeature(name);}
  
}

