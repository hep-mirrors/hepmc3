// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_SEARCH_FINDPARTICLES_H
#define  HEPMC_SEARCH_FINDPARTICLES_H
/**
 *  @file FindParticles.h
 *  @brief Definition of \b class FindParticles
 *
 *  @class HepMC::FindParticles
 *  @brief Search engine for GenEvent class
 *
 *  @ingroup search_engine
 *
 */
#include "HepMC/Search/FilterList.h"
#include "HepMC/Data/SmartPointer.h"

#include <vector>
using std::vector;

namespace HepMC {

class GenEvent;

/** @brief List of methods of searching through all particles in the event */
enum FilterEvent {
    FIND_ALL,
    FIND_FIRST,
    FIND_LAST
};

/** @brief List of methods of searching starting from particle pointer */
enum FilterParticle {
    FIND_ALL_ANCESTORS,
    FIND_ALL_DESCENDANTS,
    FIND_MOTHERS,
    FIND_DAUGHTERS,
    FIND_PRODUCTION_SIBLINGS
};

class FindParticles {
//
// Constructors
//
public:
    /** @brief GenEvent-based constructor */
    FindParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list = FilterList() );

    /** @brief GenParticle-based constructor */
    FindParticles(const GenParticlePtr &p, FilterParticle filter_type, FilterList filter_list = FilterList() );

    /** @brief Narrow down the results applying additional filters */
    void narrow_down( FilterList filter_list );
//
// Functions
//
private:
    /** @brief Check if particle passed all filters */
    bool passed_all_filters(const GenParticlePtr &p, FilterList &filter_list);

    /** @brief Check if all ancestors passed the filter
     *
     *  Recursively check all particles and production vertices of these particles
     */
    void recursive_check_ancestors(const GenVertexPtr &v, FilterList &filter_list);

    /** @brief Check if all descendants passed the filter
     *
     *  Recursively check all particles and end vertices of these particles
     */
    void recursive_check_descendants(const GenVertexPtr &v, FilterList &filter_list);
//
// Accessors
//
public:
    const vector<GenParticlePtr>& results() const { return m_results; } //!< Get results

//
// Fields
//
private:
    vector<GenParticlePtr> m_results;          //!< List of results
    vector<GenVertexPtr>   m_checked_vertices; //!< List of already checked vertices
};

} // namespace HepMC

#endif
