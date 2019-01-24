// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_PRINT_H
#define HEPMC3_PRINT_H
///
/// @file Print.h
/// @brief Definition of static \b class Print
///

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

namespace HepMC3 {


class GenPdfInfo;
class GenHeavyIon;
class GenCrossSection;


/// @brief Provides different printing formats
/// @todo This class has no state -- why isn't it just a namespace with free functions?
class Print {
public:
    /// @brief Print content of all GenEvent containers
    static void content(const GenEvent &event);

    /// @brief Print event in listing (HepMC2) format
    static void listing(const GenEvent &event, unsigned short precision = 2);

    /// @brief Print one-line info
    static void line(const GenEvent &event, const bool& attributes=false);

    /// @brief Print one-line info
    static void line(ConstGenVertexPtr v, const bool& attributes=false);

    /// @brief Print one-line info
    static void line(ConstGenParticlePtr p, const bool& attributes=false);

    /// @brief Print one-line info
    static void line(shared_ptr<GenCrossSection> &cs);

    /// @brief Print one-line info
    static void line(shared_ptr<GenHeavyIon> &hi);

    /// @brief Print one-line info
    static void line(shared_ptr<GenPdfInfo> &pi);

private:
    /// @brief Helper function for printing a vertex in listing format
    static void listing(ConstGenVertexPtr v);

    /// @brief Helper function for printing a particle in listing format
    static void listing(ConstGenParticlePtr p);

    virtual ~Print() {}
};


} // namespace HepMC3

#endif
