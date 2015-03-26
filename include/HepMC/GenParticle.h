// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_GENPARTICLE_H
#define  HEPMC_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class GenParticle
 *
 *  @class HepMC::GenParticle
 *  @brief Stores particle-related information
 *
 */
#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Data/GenParticleData.h"
#include "HepMC/FourVector.h"
#include <iostream>
#include <vector>

namespace HepMC {

class GenEvent;
class Attribute;

class GenParticle {

friend class GenEvent;
friend class GenVertex;
friend class SmartPointer<GenParticle>;

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenParticle( const FourVector &momentum = FourVector::ZERO_VECTOR(), int pdg_id = 0, int status = 0 );

    /** @brief Constructor based on particle data */
    GenParticle( const GenParticleData &data );

//
// Functions
//
public:
    /** @brief Check if this particle belongs to an event */
    bool in_event() const { return (bool)(m_event); }

//
// Accessors
//
public:
    GenEvent*              parent_event() const { return m_event; } //!< Get parent event
    int                    id()           const { return m_id;    } //!< Get particle id
    const GenParticleData& data()         const { return m_data;  } //!< Get particle data

    int   pdg_id()                const { return m_data.pdg_id;         } //!< Get PDG ID
    int   status()                const { return m_data.status;         } //!< Get status code
    const FourVector& momentum()  const { return m_data.momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data.is_mass_set;    } //!< Check if generated mass is set

    /** @brief Get generated mass
     *
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    const GenVertexPtr production_vertex() const;        //!< Get production vertex
    const GenVertexPtr end_vertex() const;               //!< Get end vertex

    GenVertexPtr production_vertex();        //!< Get production vertex
    GenVertexPtr end_vertex();               //!< Get end vertex

    void set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void set_status(int status);                   //!< Set status code
    void set_momentum(const FourVector& momentum); //!< Set momentum
    void set_generated_mass(double m);             //!< Set generated mass
    void unset_generated_mass();                   //!< Declare that generated mass is not set

    /** @brief Add event attribute to this particle
     *
     *  This will overwrite existing attribute if an attribute with
     *  the same name is present. The attribute will be stored in the
     *  parent_event(). @return false if there is no parent_event();
     */
    bool add_attribute(std::string name, shared_ptr<Attribute> att);

    /// Remove attribute
    void remove_attribute(std::string name);

    /// Get attribute of type T
    template<class T>
    shared_ptr<T> attribute(std::string name) const;

//
// Fields
//
private:
    GenEvent        *m_event; //!< Parent event
    int              m_id;    //!< Index
    GenParticleData  m_data;  //!< Particle data

    weak_ptr<GenVertex>    m_production_vertex; //!< Production vertex
    weak_ptr<GenVertex>    m_end_vertex;        //!< End vertex
    weak_ptr<GenParticle>  m_this;              //!< Pointer to shared pointer managing this particle
};

} // namespace HepMC

#include "HepMC/GenEvent.h"

template<class T>
HepMC::shared_ptr<T> HepMC::GenParticle::attribute(std::string name) const {
  return parent_event()?
    parent_event()->attribute<T>(name, id()): HepMC::shared_ptr<T>();
}

#endif
