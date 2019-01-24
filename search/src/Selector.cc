// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/Selector.h"

namespace HepMC3 {
  const SelectorWrapper<int> Selector::STATUS      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->status();});
  const SelectorWrapper<int> Selector::PDG_ID      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->pdg_id();});
  const SelectorWrapper<double> Selector::PT       = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().pt();});
  const SelectorWrapper<double> Selector::RAPIDITY = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().rap();});
  
  
    ConstSelectorPtr abs(const Selector &input)
  {
    return input.abs();
  }

  
  AttributeFeature Selector::ATTRIBUTE(const std::string &name){return AttributeFeature(name);}
  
}

