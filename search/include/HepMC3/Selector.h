// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Selector.h
/// @brief definition of /b Selector class
///
#ifndef HEPMC3_SELECTOR_H
#define HEPMC3_SELECTOR_H

#include "HepMC3/Filter.h"
#include "HepMC3/Feature.h"
#include "HepMC3/AttributeFeature.h"

namespace HepMC3 {
/** @brief  Forward declaration of SelectorWrapper */
template<typename T>
class SelectorWrapper;

class Selector;
/** @brief Declaration of ConstSelectorPtr */
using ConstSelectorPtr = std::shared_ptr<const Selector>;

/**
 *  @brief Selector is an interface to "standard" Features that are valid
 *  for both integral and floating point comparisons
 *
 *  You would use this in preference to the more general
 *  Feature<> templated type.  A Selector is constructed from a
 *  function to extract features from particles, e.g.
 *
 *  ConstSelectorPtr status = std::make_shared<SelectorWrapper<int> >([](ConstParticlePtr p)->int{return p->status();});
 *  ConstSelectorPtr pt = std::make_shared<SelectorWrapper<double> >([](ConstParticlePtr p)->double{return p->momentum().pt();});
 *
 *  You can then use the Selector to construct Filter functions that
 *  evaluate on particles, e.g.
 *  Filter is_stable = (*status) == 1;
 *  bool stable = is_stable(p);
 *  bool beam = (*status == 4)(p);
 *
 *  Selector contains a few standard Selectors already defined, e.g.
 *
 *  ConstGenParticlePtr p;
 *  (Selector::STATUS == 1)(p);
 *  (Selector::PT > 15.)(p);
 *  (abs(Selector::RAPIDITY) < 2.5)(p);
 *
 *  you can also combined them e.g.
 *
 *  Filter myCuts = (Selector::PT > 15.) && (*abs(Selector::RAPIDITY) < 2.5) || (Selector::PT > 100.);
 *  bool passCuts = myCuts(p);
 */
class Selector {

public:

    virtual ~Selector() {};

    virtual Filter operator > (int value) const = 0;
    virtual Filter operator > (double value) const = 0;

    virtual Filter operator >= (int value) const = 0;
    virtual Filter operator >= (double value) const = 0;

    virtual Filter operator < (int value) const = 0;
    virtual Filter operator < (double value) const = 0;

    virtual Filter operator <= (int value) const = 0;
    virtual Filter operator <= (double value) const = 0;

    virtual Filter operator == (int value) const = 0;
    virtual Filter operator == (double value) const = 0;

    virtual Filter operator != (int value) const = 0;
    virtual Filter operator != (double value) const = 0;

    virtual ConstSelectorPtr abs() const = 0;

    static const SelectorWrapper<int>    STATUS;
    static const SelectorWrapper<int>    PDG_ID;
    static const SelectorWrapper<double> PT;
    static const SelectorWrapper<double> ENERGY;
    static const SelectorWrapper<double> RAPIDITY;
    static const SelectorWrapper<double> ETA;
    static const SelectorWrapper<double> PHI;
    static const SelectorWrapper<double> ET;
    static const SelectorWrapper<double> MASS;
    static AttributeFeature ATTRIBUTE(const std::string &name);

};
/** @brief  SelectorWrapper */
template<typename Feature_type>
class SelectorWrapper : public Selector {

public:

    SelectorWrapper(typename Feature<Feature_type>::Evaluator_type functor): m_internal(functor) {}

    Filter operator > (int value) const override {
        return m_internal > value;
    }

    Filter operator > (double value) const override {
        return m_internal > value;
    }

    Filter operator >= (int value) const override {
        return m_internal >= value;
    }

    Filter operator >= (double value) const override {
        return m_internal >= value;
    }

    Filter operator < (int value) const override {
        return m_internal < value;
    }

    Filter operator < (double value) const override {
        return m_internal < value;
    }

    Filter operator <= (int value) const override {
        return m_internal <= value;
    }

    Filter operator <= (double value) const override {
        return m_internal <= value;
    }

    Filter operator == (int value) const override {
        return m_internal == value;
    }

    Filter operator == (double value) const override {
        return m_internal == value;
    }

    Filter operator != (int value) const override {
        return m_internal != value;
    }

    Filter operator != (double value) const override {
        return m_internal != value;
    }

    ConstSelectorPtr abs() const override {
        SelectorWrapper<Feature_type> *copy = new SelectorWrapper<Feature_type>(*this);
        copy->m_internal = m_internal.abs();
        return ConstSelectorPtr(copy);
    }

private:

    Feature<Feature_type> m_internal;

};
/** @brief ConstSelectorPtr abs*/
ConstSelectorPtr abs(const Selector &input);

}
#endif
