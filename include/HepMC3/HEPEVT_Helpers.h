// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPEVT_HELPERS_H
#define HEPEVT_HELPERS_H
/**
 *  @file HEPEVT_Helpers.h
 *  @brief Sorting functions used to convert HEPEVT block
 *
 */
#include <algorithm>
#include <map>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
namespace HepMC3
{
/** @brief comparison of two particles */
struct GenParticlePtr_greater
{
    /** @brief comparison of two particles */
    bool operator()(ConstGenParticlePtr lx, ConstGenParticlePtr rx) const;
};
/** @brief  Order vertices with equal paths. */
struct pair_GenVertexPtr_int_greater
{
    /** @brief  Order vertices with equal paths. If the paths are equal, order in other quantities.
     * We cannot use id, as it can be assigned in different way*/
    bool operator()(const std::pair<ConstGenVertexPtr, int>& lx, const std::pair<ConstGenVertexPtr, int>& rx) const;
};
/** @brief Calculates the path to the top (beam) particles */
void calculate_longest_path_to_top(ConstGenVertexPtr v, std::map<ConstGenVertexPtr, int>& pathl);
}
#endif
