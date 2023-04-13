// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Relatives.h
/// @brief Defines helper classes to extract relatives of an input GenParticle or GenVertex
///
#ifndef HEPMC3_RELATIVES_H
#define HEPMC3_RELATIVES_H
#if defined(WIN32)&&(!defined(HEPMC3search_NO_Relatives_EXPORTS))
#ifdef HepMC3search_EXPORTS
#define HEPMC3search_Relatives_EXPORT_API  __declspec(dllexport)
#else
#define HEPMC3search_Relatives_EXPORT_API  __declspec(dllimport)
#endif
#else
#define HEPMC3search_Relatives_EXPORT_API
#endif


#include <vector>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
namespace HepMC3 {
std::vector<HepMC3::GenParticlePtr>      children_particles(const HepMC3::GenVertexPtr& O);   ///< Return children particles
std::vector<HepMC3::ConstGenParticlePtr> children_particles(const HepMC3::ConstGenVertexPtr& O); ///< Return children particles
std::vector<HepMC3::GenVertexPtr>        children_vertices(const HepMC3::GenParticlePtr& O); ///< Return children vertices
std::vector<HepMC3::ConstGenVertexPtr>   children_vertices(const HepMC3::ConstGenParticlePtr& O); ///< Return children vertices
std::vector<HepMC3::GenParticlePtr>      grandchildren_particles(const HepMC3::GenParticlePtr& O);  ///< Return grandchildren particles
std::vector<HepMC3::ConstGenParticlePtr> grandchildren_particles(const HepMC3::ConstGenParticlePtr& O);  ///< Return grandchildren particles
std::vector<HepMC3::GenVertexPtr>        grandchildren_vertices(const HepMC3::GenVertexPtr& O);   ///< Return grandchildren vertices
std::vector<HepMC3::ConstGenVertexPtr>   grandchildren_vertices(const HepMC3::ConstGenVertexPtr& O); ///< Return grandchildren vertices
std::vector<HepMC3::GenParticlePtr>      parent_particles(const HepMC3::GenVertexPtr& O);  ///< Return parent particles
std::vector<HepMC3::ConstGenParticlePtr> parent_particles(const HepMC3::ConstGenVertexPtr& O);   ///< Return parent particles
std::vector<HepMC3::GenVertexPtr>        parent_vertices(const HepMC3::GenParticlePtr& O);   ///< Return parent vertices
std::vector<HepMC3::ConstGenVertexPtr>   parent_vertices(const HepMC3::ConstGenParticlePtr& O);    ///< Return parent vertices
std::vector<HepMC3::GenParticlePtr>      grandparent_particles(const HepMC3::GenParticlePtr& O);    ///< Return grandparent particles
std::vector<HepMC3::ConstGenParticlePtr> grandparent_particles(const HepMC3::ConstGenParticlePtr& O);     ///< Return grandparent particles
std::vector<HepMC3::GenVertexPtr>        grandparent_vertices(const HepMC3::GenVertexPtr& O);      ///< Return grandparent vertices
std::vector<HepMC3::ConstGenVertexPtr>   grandparent_vertices(const HepMC3::ConstGenVertexPtr& O);       ///< Return grandparent vertices
std::vector<HepMC3::ConstGenParticlePtr> descendant_particles(const HepMC3::ConstGenVertexPtr& obj);       ///< Return descendant particles
std::vector<HepMC3::GenParticlePtr>      descendant_particles(const HepMC3::GenVertexPtr& obj);       ///< Return descendant particles
std::vector<HepMC3::ConstGenParticlePtr> descendant_particles(const HepMC3::ConstGenParticlePtr& obj);       ///< Return descendant particles
std::vector<HepMC3::GenParticlePtr>      descendant_particles(const HepMC3::GenParticlePtr& obj);       ///< Return descendant particles
std::vector<HepMC3::ConstGenVertexPtr>   descendant_vertices(const HepMC3::ConstGenParticlePtr& obj);       ///< Return descendant vertices
std::vector<HepMC3::GenVertexPtr>        descendant_vertices(const HepMC3::GenParticlePtr& obj);       ///< Return descendant vertices
std::vector<HepMC3::ConstGenVertexPtr>   descendant_vertices(const HepMC3::ConstGenVertexPtr& obj);       ///< Return descendant vertices
std::vector<HepMC3::GenVertexPtr>        descendant_vertices(const HepMC3::GenVertexPtr& obj);       ///< Return descendant vertices
std::vector<HepMC3::ConstGenParticlePtr> ancestor_particles(const HepMC3::ConstGenVertexPtr& obj);       ///< Return ancestor particles
std::vector<HepMC3::GenParticlePtr>      ancestor_particles(const HepMC3::GenVertexPtr& obj);      ///< Return ancestor particles
std::vector<HepMC3::ConstGenParticlePtr> ancestor_particles(const HepMC3::ConstGenParticlePtr& obj);      ///< Return ancestor particles
std::vector<HepMC3::GenParticlePtr>      ancestor_particles(const HepMC3::GenParticlePtr& obj);      ///< Return ancestor particles
std::vector<HepMC3::ConstGenVertexPtr>   ancestor_vertices(const HepMC3::ConstGenParticlePtr& obj);      ///< Return ancestor vertices
std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(const HepMC3::GenParticlePtr& obj);      ///< Return ancestor vertices
std::vector<HepMC3::ConstGenVertexPtr>   ancestor_vertices(const HepMC3::ConstGenVertexPtr& obj);      ///< Return ancestor vertices
std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(const HepMC3::GenVertexPtr& obj);      ///< Return ancestor vertices
}



namespace HepMC3 {

class SearchParents {
public:
 std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input) { return parent_particles(input);}
 std::vector<GenParticlePtr> operator()(GenVertexPtr input) { return parent_particles(input);}
 std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input) { return grandparent_particles(input);}
 std::vector<GenParticlePtr> operator()(GenParticlePtr input) { return grandparent_particles(input);}
};

class SearchChildren {
public:
 std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input)const  { return children_particles(input);}
 std::vector<GenParticlePtr> operator()(GenVertexPtr input)const  { return children_particles(input);}
 std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input)const  { return grandchildren_particles(input);}
 std::vector<GenParticlePtr> operator()(GenParticlePtr input)const  { return grandchildren_particles(input);}
};

class SearchAncestors {
public:
 std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input) const { return ancestor_particles(input);}
 std::vector<GenParticlePtr> operator()(GenVertexPtr input)const  { return ancestor_particles(input);}
 std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input)const  { return ancestor_particles(input);}
 std::vector<GenParticlePtr> operator()(GenParticlePtr input)const  { return ancestor_particles(input);}
};

class SearchDescendants {
public:
 std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input)const  { return descendant_particles(input);}
 std::vector<GenParticlePtr> operator()(GenVertexPtr input)const  { return descendant_particles(input);}
 std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input)const  { return descendant_particles(input);}
 std::vector<GenParticlePtr> operator()(GenParticlePtr input) const { return descendant_particles(input);}
};

/// alias
using Parents  = SearchParents;
/// alias
using Children = SearchChildren;
/// Ancestors
using Ancestors = SearchAncestors;
/// Descendants
using Descendants = SearchDescendants;

/** @brief  Define a common interface that all Relatives objects will satisfy
 *         Relatives provides an operator to get the relatives of a range of different
 *         GenObject types.  The following are examples
 *
 *         Relatives::ANCESTORS(GenParticlePtr);// returns ancestors of the particle
 *         Descendants descendants;
 *         descendants(GenVertexPtr);// descendants of the vertex
 */
class Relatives {
public:
    /// @brief Operator
    virtual std::vector<GenParticlePtr> operator()(GenParticlePtr input) const = 0;
    /// @brief Operator
    virtual std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input) const = 0;
    /// @brief Operator
    virtual std::vector<GenParticlePtr> operator()(GenVertexPtr input) const = 0;
    /// @brief Operator
    virtual std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input) const = 0;

/// The thread_local will not work with VS, see https://docs.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/compiler-error-c2492?redirectedfrom=MSDN&view=msvc-170
/// Dropping the thread_local is not what was intended initially, so instead an implementation with free functions should be used.
    HEPMC3search_Relatives_EXPORT_API static const Parents PARENTS;  ///< Parents
    HEPMC3search_Relatives_EXPORT_API static const Children CHILDREN;  ///< Children
    HEPMC3search_Relatives_EXPORT_API static const Ancestors ANCESTORS;  ///< Ancestors
    HEPMC3search_Relatives_EXPORT_API static const Descendants DESCENDANTS;  ///< Descendants
};


}
#endif

