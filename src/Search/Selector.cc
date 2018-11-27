#include "HepMC/Search/Selector.h"

namespace HepMC{
  const SelectorWrapper<int> Selector::STATUS      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->status();});
  const SelectorWrapper<int> Selector::PDG_ID      = SelectorWrapper<int>([](ConstGenParticlePtr p)->int{return p->pdg_id();});
  const SelectorWrapper<double> Selector::PT       = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().pt();});
  const SelectorWrapper<double> Selector::RAPIDITY = SelectorWrapper<double>([](ConstGenParticlePtr p)->double{return p->momentum().rap();});
}

