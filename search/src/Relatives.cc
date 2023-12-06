// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Relatives.cc
/// @brief Implementation of \b Relatives class
///
#include "HepMC3/Relatives.h"
#include <unordered_set>

namespace HepMC3 {
const Parents     Relatives::PARENTS;
const Children    Relatives::CHILDREN;
#ifdef _MSC_VER
const Ancestors   Relatives::ANCESTORS;
const Descendants Relatives::DESCENDANTS;
#else
thread_local const Ancestors   Relatives::ANCESTORS;
thread_local const Descendants Relatives::DESCENDANTS;
#endif
} // namespace HepMC3

namespace HepMC3 {
/// @brief Returns children of vertex, i.e. outgoing particles.
GenParticles      children(const GenVertexPtr& O) {
    if (O) return O->particles_out();
    return  {};
}
/// @brief Returns children of const vertex, i.e. outgoing particles.
ConstGenParticles children(const ConstGenVertexPtr& O) {
    if (O) return O->particles_out();
    return  {};
}
/// @brief Returns children of particle, i.e. the end vertex.
GenVertices        children(const GenParticlePtr& O) {
    GenVertices result;
    if (O->end_vertex()) result.emplace_back(O->end_vertex());
    return result;
}
/// @brief Returns children of const particle, i.e. the end vertex.
ConstGenVertices   children(const ConstGenParticlePtr& O) {
    ConstGenVertices result;
    if (O->end_vertex()) result.emplace_back(O->end_vertex());
    return result;
}
/// @brief Returns grandchildren of particle, i.e. the outgoing particles of the end vertex.
GenParticles      grandchildren(const GenParticlePtr& O) {
    if (O) if (O->end_vertex()) return O->end_vertex()->particles_out();
    return {};
}
/// @brief Returns grandchildren of const particle, i.e. the outgoing particles of the end vertex.
ConstGenParticles grandchildren(const ConstGenParticlePtr& O) {
    if (O) if (O->end_vertex()) return O->end_vertex()->particles_out();
    return {};
}
/// @brief Returns grandchildren of vertex, i.e. the end vertices of the outgoing particles.
GenVertices        grandchildren(const GenVertexPtr& O) {
    GenVertices result;
    if (O) for (const auto&o: O->particles_out()) if (o->end_vertex()) result.emplace_back(o->end_vertex());
    return result;
}
/// @brief Returns grandchildren of const vertex, i.e. the end vertices of the outgoing particles.
ConstGenVertices   grandchildren(const ConstGenVertexPtr& O) {
    ConstGenVertices result;
    if (O)  for (const auto& o:O->particles_out()) if (o->end_vertex()) result.emplace_back(o->end_vertex());
    return result;
}
/// @brief Returns parents of vertex, i.e. incoming particles.
GenParticles      parents(const GenVertexPtr& O) {
    if (O) return O->particles_in();
    return  {};
}
/// @brief Returns parents of const vertex, i.e. incoming particles.
ConstGenParticles parents(const ConstGenVertexPtr& O) {
    if (O) return O->particles_in();
    return  {};
}
/// @brief Returns parents of particle, i.e. production vertex.
GenVertices        parents(const GenParticlePtr& O) {
    GenVertices result;
    if (O->production_vertex()) result.emplace_back(O->production_vertex());
    return result;
}
/// @brief Returns parents of const particle, i.e. production vertex.
ConstGenVertices   parents(const ConstGenParticlePtr& O) {
    ConstGenVertices result;
    if (O->production_vertex()) result.emplace_back(O->production_vertex());
    return result;
}
/// @brief Returns grandparents of particle, i.e. incoming particles of production vertex.
GenParticles      grandparents(const GenParticlePtr& O) {
    if (O) if (O->production_vertex()) return O->production_vertex()->particles_in();
    return {};
}
/// @brief Returns grandparents of const particle, i.e. incoming particles of production vertex.
ConstGenParticles grandparents(const ConstGenParticlePtr& O) {
    if (O) if (O->production_vertex()) return O->production_vertex()->particles_in();
    return {};
}
/// @brief Returns grandparents of vertex, i.e. production vertices of incoming particles.
GenVertices        grandparents(const GenVertexPtr& O) {
    GenVertices result;
    if (O) for (const auto& o: O->particles_in()) if (o->production_vertex()) result.emplace_back(o->production_vertex());
    return result;
}
/// @brief Returns grandparents of const vertex, i.e. production vertices of incoming particles.
ConstGenVertices   grandparents(const ConstGenVertexPtr& O) {
    ConstGenVertices result;
    if (O)  for (const auto& o: O->particles_in()) if (o->end_vertex()) result.emplace_back(o->production_vertex());
    return result;
}
/// @brief Returns descendands of the same type, i.e. vertices for vertex and particles for particle
template <class O>  std::list<O> descendants_of_same_type(const O& obj)
{
    std::list<O>           result = grandchildren(obj);
    auto                   cur    = result.begin();
    std::unordered_set<O>  seen(cur, result.end());
    
    // As long as the results container grows 
    while (cur != result.end()) {
      std::list<O> temp;
      for (; cur != result.end(); ++cur) {
        auto  temp0 = grandchildren(*cur);
        std::move(temp0.begin(), temp0.end(), std::back_inserter(temp));
      }
      
      // Rewind curser one step 
      auto prev = std::prev(cur);

      // Add found particles not already seen 
      for (const auto& p2: temp) {
        if (seen.find(p2) == seen.end()) {
          // This changes the size of results, and thus the while
          // condition may stil be true.  E.g., after the above loop,
          // cur will point to results.end(), but here we add more, so
          // that cur no longer points to results.end().
          result.emplace_back(p2);
          seen.insert(p2);
        }
      }
      // Increment the cursor after adding to the results so that it
      // no longer points to the end.
      cur = std::next(prev);
    }

    return result;
}
/// @brief Returns descendands of the other type, i.e. vertices for  particle and particles for vertex
template <class O, class R>  std::list<R> descendants_of_other_type(const O& obj)
{
    std::list<R> localchildren = children(obj);
    std::list<R>  result = localchildren;
    for (const auto& c: localchildren)
    {
        std::list<R> desc = descendants_of_same_type(c);
        for (const auto& d: desc) if (std::find(result.begin(), result.end(), d) == result.end()) result.emplace_back(d);
    }
    return result;
}
/// @brief Returns ancestors of the same type, i.e. vertices for vertex and particles for particle
template <class O>  std::list<O> ancestors_of_same_type(const O& obj)
{
    std::list<O>  result = grandparents(obj);
    std::unordered_set<O>  seen(result.begin(), result.end());
    auto          cur    = result.begin();

    // As long as the results container grows 
    while (cur != result.end()) {
      std::list<O>  temp;
      for (; cur != result.end(); ++cur) {
        auto  temp0 = grandparents(*cur);
        std::move(temp0.begin(), temp0.end(), std::back_inserter(temp));
      }

      // Rewind curser one step 
      auto prev = std::prev(cur);

      // Add found particles not already seen       
      for (const auto& p2: temp) {
        if (seen.find(p2) == seen.end()) {
          // This changes the size of results, and thus the while
          // condition may stil be true.  E.g., after the above loop,
          // cur will point to results.end(), but here we add more, so
          // that cur no longer points to results.end().
          result.emplace_back(p2);
          seen.insert(p2);
        }
      }
      // Increment the cursor after adding to the results so that it
      // no longer points to the end.
      cur = std::next(prev);
    }
    return result;
}

/// @brief Returns ancestors of the other type, i.e. vertices for  particle and particles for vertex
template <class O, class R>  std::list<R> ancestors_of_other_type(const O& obj)
{
    std::list<R> localparents = parents(obj);
    std::list<R>  result = localparents;
    for (const auto& c: localparents)
    {
        std::list<R> desc = ancestors_of_same_type(c);
        for (const auto& d: desc) if (std::find(result.begin(), result.end(), d) == result.end()) result.emplace_back(d);
    }
    return result;
}

ConstGenParticles descendant_particles(const ConstGenVertexPtr& obj) {
    return  descendants_of_other_type<ConstGenVertexPtr, ConstGenParticlePtr>(obj);
}
GenParticles descendant_particles(const GenVertexPtr& obj) {
    return descendants_of_other_type<GenVertexPtr, GenParticlePtr>(obj);
}

ConstGenVertices descendant_vertices(const ConstGenParticlePtr& obj) {
    return descendants_of_other_type<ConstGenParticlePtr, ConstGenVertexPtr>(obj);
}
GenVertices descendant_vertices(const GenParticlePtr& obj) {
    return descendants_of_other_type<GenParticlePtr, GenVertexPtr>(obj);
}

ConstGenParticles ancestor_particles(const ConstGenVertexPtr& obj) {
    return  ancestors_of_other_type<ConstGenVertexPtr, ConstGenParticlePtr>(obj);
}
GenParticles ancestor_particles(const GenVertexPtr& obj) {
    return ancestors_of_other_type<GenVertexPtr, GenParticlePtr>(obj);
}

ConstGenVertices ancestor_vertices(const ConstGenParticlePtr& obj) {
    return ancestors_of_other_type<ConstGenParticlePtr, ConstGenVertexPtr>(obj);
}
GenVertices ancestor_vertices(const GenParticlePtr& obj) {
    return ancestors_of_other_type<GenParticlePtr, GenVertexPtr>(obj);
}


ConstGenParticles  descendant_particles(const ConstGenParticlePtr& obj)  { return descendants_of_same_type(obj); }
GenParticles       descendant_particles(const GenParticlePtr& obj)       { return descendants_of_same_type(obj); }
ConstGenVertices   descendant_vertices(const ConstGenVertexPtr& obj)     { return descendants_of_same_type(obj); }
GenVertices        descendant_vertices(const GenVertexPtr& obj)          { return descendants_of_same_type(obj); }
ConstGenParticles  ancestor_particles(const ConstGenParticlePtr& obj)    { return ancestors_of_same_type(obj); }
GenParticles       ancestor_particles(const GenParticlePtr& obj)         { return ancestors_of_same_type(obj); }
ConstGenVertices   ancestor_vertices(const ConstGenVertexPtr& obj)       { return ancestors_of_same_type(obj); }
GenVertices        ancestor_vertices(const GenVertexPtr& obj)            { return ancestors_of_same_type(obj); }
GenParticles       children_particles(const GenVertexPtr& O)             { return children(O); }
ConstGenParticles  children_particles(const ConstGenVertexPtr& O)        { return children(O); }
GenVertices        children_vertices(const GenParticlePtr& O)            { return children(O); }
ConstGenVertices   children_vertices(const ConstGenParticlePtr& O)       { return children(O); }
GenParticles       grandchildren_particles(const GenParticlePtr& O)      { return grandchildren(O); }
ConstGenParticles  grandchildren_particles(const ConstGenParticlePtr& O) { return grandchildren(O); }
GenVertices        grandchildren_vertices(const GenVertexPtr& O)         { return grandchildren(O); }
ConstGenVertices   grandchildren_vertices(const ConstGenVertexPtr& O)    { return grandchildren(O); }
GenParticles       parent_particles(const GenVertexPtr& O)               { return parents(O); }
ConstGenParticles  parent_particles(const ConstGenVertexPtr& O)          { return parents(O); }
GenVertices        parent_vertices(const GenParticlePtr& O)              { return parents(O); }
ConstGenVertices   parent_vertices(const ConstGenParticlePtr& O)         { return parents(O); }
GenParticles       grandparent_particles(const GenParticlePtr& O)        { return grandparents(O); }
ConstGenParticles  grandparent_particles(const ConstGenParticlePtr& O)   { return grandparents(O); }
GenVertices        grandparent_vertices(const GenVertexPtr& O)           { return grandparents(O); }
ConstGenVertices   grandparent_vertices(const ConstGenVertexPtr& O)      { return grandparents(O); }


} // namespace HepMC3
