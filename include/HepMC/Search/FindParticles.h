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

#include "HepMC/GenParticle.fh"
#include "HepMC/GenVertex.fh"

namespace HepMC {


class GenEvent;


/** @brief List of methods of searching through all particles in the event */
enum FilterType {
    FIND_ALL,
    FIND_FIRST,
    FIND_LAST
};

/** @brief Generic findParticles function declaration
 * There is no generic implementation, just the specialisations below
 */
template<typename T, typename U>
std::vector<std::shared_ptr<U> > findParticles(std::shared_ptr<T> input, Relationship filter_type, FilterList filter_list = FilterList());

/** @brief Finds Particles that have a given relationship to an input particle and (optionally) pass a list of filters
 *
 *  @details This is a specialisation of the generic findParticles function that operates on a ParticlePtr or a ConstParticlePtr.
 *           If ParticlePtr is used, then a vector<ParticlePtr> is returned.
 *           If ConstParticlePtr is input, then a vector<ConstParticlePtr> is returned
 *
 *  @param input: the ParticlePtr or ConstParticlePtr where the search is begun
 *  @param filter_type: The relationship (ANCESTOR, DESCENDANT etc.) between the sought particles and the input ParticlePtr
 *  @param filter_list: a list of filters that the sought particles must satisfy
 */

template <typename T, typename U=typename std::conditional<std::is_const<T>::value, const GenParticle, GenParticle >::type>
typename std::enable_if<std::is_same<GenParticle, typename std::remove_const<T>::type>::value, std::vector<std::shared_ptr<U> > >::type findParticles(std::shared_ptr<T> input, Relationship filter_type, FilterList filter_list = FilterList()){

    std::vector<std::shared_ptr<U> > results;

    return results;
}

/** @brief Finds Particles that have a given relationship to an input vertex and (optionally) pass a list of filters
 *
 *  @details This is a specialisation of the generic findParticles function that operates on a VertexPtr or a ConstVertexPtr.
 *           If VertexPtr is used, then a vector<ParticlePtr> is returned.
 *           If ConstVertexPtr is input, then a vector<ConstParticlePtr> is returned
 *
 *  @param input: the VertexPtr or ConstVertexPtr where the search is begun
 *  @param filter_type: The relationship (ANCESTOR, DESCENDANT etc.) between the sought particles and the input VertexPtr
 *  @param filter_list: a list of filters that the sought particles must satisfy
 */

template <typename T, typename U=typename std::conditional<std::is_const<T>::value, const GenParticle, GenParticle >::type>
typename std::enable_if<std::is_same<GenVertex, typename std::remove_const<T>::type>::value, std::vector<std::shared_ptr<U> > >::type findParticles(std::shared_ptr<T> input, Relationship filter_type, FilterList filter_list = FilterList()){

    std::vector<std::shared_ptr<U> > results;

    return results;

}

template <typename T, typename U=typename std::conditional<std::is_const<T>::value, const GenParticle, GenParticle >::type>
typename std::enable_if<std::is_same<GenEvent, typename std::remove_const<typename std::remove_reference<T>::type>::type>::value, std::vector<std::shared_ptr<U> > >::type findParticles(std::shared_ptr<T> input, Relationship filter_type, FilterList filter_list = FilterList()){

    std::vector<std::shared_ptr<U> > results;

    return results;

}
  
/// Compatibility name
using FilterEvent = FilterType;


class FindParticles {
//
// Constructors
//
public:

    /** @brief GenEvent-based constructor */
    FindParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list = FilterList() );

    /** @brief GenParticle-based constructor */
    FindParticles(ConstGenParticlePtr p, Relationship filter_type, FilterList filter_list = FilterList() );

    /** @brief GenVertex-based constructor */
    FindParticles(ConstGenVertexPtr v, Relationship filter_type, FilterList filter_list = FilterList() );

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
    const vector<ConstGenParticlePtr>& results() const { return m_results; } //!< Get results

//
// Fields
//
private:
    vector<ConstGenParticlePtr> m_results;          //!< List of results
    vector<ConstGenVertexPtr>   m_checked_vertices; //!< List of already checked vertices
};


  /// @name Finding via unbound functions (returns by copy)
  //@{

  /** @brief Find from GenEvent */
//  inline vector<ConstGenParticlePtr> findParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list = FilterList() ) {
  //  return FindParticles(evt, filter_type, filter_list).results();
  //}

  /** @brief Find from GenParticle */
  
  //inline vector<GenParticlePtr> findParticles(GenParticlePtr p, Relationship filter_type, FilterList filter_list = FilterList() ) {
//    return FindParticles(p, filter_type, filter_list).results();
//  }

  /** @brief Find from GenParticle */
//  inline vector<ConstGenParticlePtr> findParticles(ConstGenParticlePtr p, Relationship filter_type, FilterList filter_list = FilterList() ) {
  //  return FindParticles(p, filter_type, filter_list).results();
  //}

  /** @brief Find from GenVertex */
  /*
  inline vector<GenParticlePtr> findParticles(GenVertexPtr v, Relationship filter_type, FilterList filter_list = FilterList() ) {
    return FindParticles(v, filter_type, filter_list).results();
  }
*/
  /** @brief Find from GenVertex */
  //inline vector<ConstGenParticlePtr> findParticles(ConstGenVertexPtr v, Relationship filter_type, FilterList filter_list = FilterList() ) {
    //return FindParticles(v, filter_type, filter_list).results();
  //}

  //@}


} // namespace HepMC

#endif
