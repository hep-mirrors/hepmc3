// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file Relatives.h
/// @brief Defines helper classes to extract relatives of an input GenParticle or GenVertex
///
#ifndef HEPMC3_RELATIVES_H
#define HEPMC3_RELATIVES_H

#include <vector>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"


namespace HepMC3 {

/// forward declare the Relatives interface in which _parents and _children are wrapped
template<typename T>
class RelativesInterface;
/// forward declare the recursion wrapper
template<typename T>
class Recursive;

// forward declare _parents class
class _parents;
// forward declare _children class
class _children;

/// alias of _parents wrapped in the Relatives interface
using Parents  = RelativesInterface<_parents>;
/// alias of _children wrapped in the Relatives interface
using Children = RelativesInterface<_children>;
/// Ancestors is an alias to Recursion applied to the _parents and wrapped in the Relatives interface
using Ancestors = RelativesInterface<Recursive<_parents> >;
/// Descendants is an alias to Recursion applied to the _children and wrapped in the Relatives interface
using Descendants = RelativesInterface<Recursive<_children> >;

/** @brief  Define a common interface that all Relatives objects will satisfy
 *         Relatives provides an operator to get the relatives of a range of different
 *         GenObject types.  The following are examples
 *
 *         Relatives::ANCESTORS(GenParticlePtr);// returns ancestors of the particle
 *         Descendants descendants;
 *         descendants(GenVertexPtr);// descendants of the vertex
 *         vector<Relatives*> relations = {&Relatives::CHILDREN, &Relatives::DESCENDANTS, &Relatives::PARENTS, new Ancestors()}; // make a vector of Relatives
 *
 *         You can also define your own relation and wrap it in the Relatives interface using
 *         Relatives * relo = new RelativesInterface<MyRelationClass>();
 */
class Relatives {
public:
    virtual std::vector<GenParticlePtr> operator()(GenParticlePtr input) const = 0;
    virtual std::vector<ConstGenParticlePtr> operator()(ConstGenParticlePtr input) const = 0;
    virtual std::vector<GenParticlePtr> operator()(GenVertexPtr input) const = 0;
    virtual std::vector<ConstGenParticlePtr> operator()(ConstGenVertexPtr input) const = 0;

    static const Parents PARENTS;
    static const Children CHILDREN;
    static thread_local const Ancestors ANCESTORS;
    static thread_local const Descendants DESCENDANTS;
};

/** @brief wrap a templated class that implements Relatives
 *  Since we need to template the functionality on the input
 *  type (GenParticlePtr, ConstGenVertexPtr etc.) we must wrap a
 *  class that has a templated operator in this that provides the
 *  Relatives interface and calls through to the underlying template
 *  method.
 */
template<typename Relative_type>
class RelativesInterface : public Relatives {
public:
    //RelativesInterface(Relative_type relatives): _internal(relatives){}
    constexpr RelativesInterface() {}

    GenParticles_type<GenParticlePtr> operator()(GenParticlePtr input) const override {return _internal(input);}
    GenParticles_type<ConstGenParticlePtr> operator()(ConstGenParticlePtr input) const override {return _internal(input);}
    GenParticles_type<GenVertexPtr> operator()(GenVertexPtr input) const override {return _internal(input);}
    GenParticles_type<ConstGenVertexPtr> operator()(ConstGenVertexPtr input) const override {return _internal(input);}

private:
    Relative_type _internal;
};
/** @brief  Recursive */
template<typename Relation_type>
class Recursive {
public:
    template<typename GenObject_type>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {
        for (auto obj: m_checkedObjects) {
            delete obj;
        }
        m_checkedObjects.clear();
        return _recursive(input);
    }

private:
    template<typename GenObject_type, typename dummy>
    GenParticles_type<GenObject_type> _recursive(GenObject_type input) const;

    GenParticles_type<GenVertexPtr> _recursive(GenVertexPtr input) const {
        GenParticles_type <GenVertexPtr> results;
        if ( !input ) return results;
        for (auto v: m_checkedObjects) {
            if (v->id() == input->id()) return results;
        }

        m_checkedObjects.emplace_back(new idInterface<GenVertexPtr>(input));

        for (auto p: m_applyRelation(input)) {
            results.emplace_back(p);
            GenParticles_type <GenVertexPtr> tmp = _recursive(p);
            results.insert(results.end(),
                           std::make_move_iterator(tmp.begin()),
                           std::make_move_iterator(tmp.end()));
        }

        return results;
    }

    GenParticles_type<ConstGenVertexPtr> _recursive(ConstGenVertexPtr input) const {
        GenParticles_type <ConstGenVertexPtr> results;
        if ( !input ) return results;
        for (auto v: m_checkedObjects) {
            if (v->id() == input->id()) return results;
        }

        m_checkedObjects.emplace_back(new idInterface<ConstGenVertexPtr>(input));

        for (auto p: m_applyRelation(input)) {
            results.emplace_back(p);
            GenParticles_type <ConstGenVertexPtr> tmp = _recursive(p);
            results.insert(results.end(),
                           std::make_move_iterator(tmp.begin()),
                           std::make_move_iterator(tmp.end()));
        }

        return results;
    }

    GenParticles_type<GenParticlePtr> _recursive(GenParticlePtr input) const {
        return _recursive(m_applyRelation.vertex(input));
    }
    GenParticles_type<ConstGenParticlePtr> _recursive(ConstGenParticlePtr input) const {
        return _recursive(m_applyRelation.vertex(input));
    }


    /** @brief  hasID */
    class hasId {
    public:
        virtual ~hasId() {}
        virtual int id() const = 0;
    };
    /** @brief  iDinterface */
    template<typename ID_type>
    class idInterface : public hasId {
    public:
        constexpr idInterface(ID_type genObject): m_object(genObject) {}
        int id() const override {return m_object->id();}

    private:
        ID_type m_object;
    };

    Relation_type m_applyRelation;
    mutable std::vector<hasId*> m_checkedObjects;
};

/** @brief Provides operator to find the parent particles of a Vertex or Particle
 *
 * Note you would usually not instantiate this directly, but wrap it in a RelativesInterface
 */
class _parents {
public:
    template<typename GenObject_type, typename dummy>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const;

    template<typename GenObject_type, typename std::enable_if<std::is_same<GenVertex, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return input->particles_in();}

    template<typename GenObject_type, typename std::enable_if<std::is_same<GenParticle, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return (*this)(vertex(input));}

    template<typename GenObject_type>
    GenVertex_type<GenObject_type> vertex(GenObject_type input) const {return input->production_vertex();}
};

/** @brief Provides operator to find the child particles of a Vertex or Particle
 *
 * Note you would usually not instantiate this directly, but wrap it in a RelativesInterface
 */
class _children {
public:
    template<typename GenObject_type, typename dummy>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const;

    template<typename GenObject_type, typename std::enable_if<std::is_same<GenVertex, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return input->particles_out();}

    template<typename GenObject_type, typename std::enable_if<std::is_same<GenParticle, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return (*this)(vertex(input));}

    template<typename GenObject_type>
    GenVertex_type<GenObject_type> vertex(GenObject_type input) const {return input->end_vertex();}
};

}


namespace HepMC3 {

std::vector<HepMC3::GenParticlePtr>      children_particles(HepMC3::GenVertexPtr O);   ///< Return children particles
std::vector<HepMC3::ConstGenParticlePtr> children_particles(HepMC3::ConstGenVertexPtr O); ///< Return children particles
std::vector<HepMC3::GenVertexPtr>        children_vertices(HepMC3::GenParticlePtr O); ///< Return children vertices
std::vector<HepMC3::ConstGenVertexPtr>   children_vertices(HepMC3::ConstGenParticlePtr O); ///< Return children vertices
std::vector<HepMC3::GenParticlePtr>      grandchildren_particles(HepMC3::GenParticlePtr O);  ///< Return grandchildren particles
std::vector<HepMC3::ConstGenParticlePtr> grandchildren_particles(HepMC3::ConstGenParticlePtr O);  ///< Return grandchildren particles
std::vector<HepMC3::GenVertexPtr>        grandchildren_vertices(HepMC3::GenVertexPtr O);   ///< Return grandchildren vertices
std::vector<HepMC3::ConstGenVertexPtr>   grandchildren_vertices(HepMC3::ConstGenVertexPtr O); ///< Return grandchildren vertices
std::vector<HepMC3::GenParticlePtr>      parent_particles(HepMC3::GenVertexPtr O);  ///< Return parent particles
std::vector<HepMC3::ConstGenParticlePtr> parent_particles(HepMC3::ConstGenVertexPtr O);   ///< Return parent particles
std::vector<HepMC3::GenVertexPtr>        parent_vertices(HepMC3::GenParticlePtr O);   ///< Return parent vertices
std::vector<HepMC3::ConstGenVertexPtr>   parent_vertices(HepMC3::ConstGenParticlePtr O);    ///< Return parent vertices
std::vector<HepMC3::GenParticlePtr>      grandparent_particles(HepMC3::GenParticlePtr O);    ///< Return grandparent particles
std::vector<HepMC3::ConstGenParticlePtr> grandparent_particles(HepMC3::ConstGenParticlePtr O);     ///< Return grandparent particles
std::vector<HepMC3::GenVertexPtr>        grandparent_vertices(HepMC3::GenVertexPtr O);      ///< Return grandparent vertices
std::vector<HepMC3::ConstGenVertexPtr>   grandparent_vertices(HepMC3::ConstGenVertexPtr O);       ///< Return grandparent vertices
std::vector<HepMC3::ConstGenParticlePtr> descendant_particles(HepMC3::ConstGenVertexPtr obj);       ///< Return descendant particles
std::vector<HepMC3::GenParticlePtr>      descendant_particles(HepMC3::GenVertexPtr obj);       ///< Return descendant particles
std::vector<HepMC3::ConstGenParticlePtr> descendant_particles(HepMC3::ConstGenParticlePtr obj);       ///< Return descendant particles
std::vector<HepMC3::GenParticlePtr>      descendant_particles(HepMC3::GenParticlePtr obj);       ///< Return descendant particles
std::vector<HepMC3::ConstGenVertexPtr>   descendant_vertices(HepMC3::ConstGenParticlePtr obj);       ///< Return descendant vertices
std::vector<HepMC3::GenVertexPtr>        descendant_vertices(HepMC3::GenParticlePtr obj);       ///< Return descendant vertices
std::vector<HepMC3::ConstGenVertexPtr>   descendant_vertices(HepMC3::ConstGenVertexPtr obj);       ///< Return descendant vertices
std::vector<HepMC3::GenVertexPtr>        descendant_vertices(HepMC3::GenVertexPtr obj);       ///< Return descendant vertices
std::vector<HepMC3::ConstGenParticlePtr> ancestor_particles(HepMC3::ConstGenVertexPtr obj);       ///< Return ancestor particles
std::vector<HepMC3::GenParticlePtr>      ancestor_particles(HepMC3::GenVertexPtr obj);      ///< Return ancestor particles
std::vector<HepMC3::ConstGenParticlePtr> ancestor_particles(HepMC3::ConstGenParticlePtr obj);      ///< Return ancestor particles
std::vector<HepMC3::GenParticlePtr>      ancestor_particles(HepMC3::GenParticlePtr obj);      ///< Return ancestor particles
std::vector<HepMC3::ConstGenVertexPtr>   ancestor_vertices(HepMC3::ConstGenParticlePtr obj);      ///< Return ancestor vertices
std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(HepMC3::GenParticlePtr obj);      ///< Return ancestor vertices
std::vector<HepMC3::ConstGenVertexPtr>   ancestor_vertices(HepMC3::ConstGenVertexPtr obj);      ///< Return ancestor vertices
std::vector<HepMC3::GenVertexPtr>        ancestor_vertices(HepMC3::GenVertexPtr obj);      ///< Return ancestor vertices
}

#endif

