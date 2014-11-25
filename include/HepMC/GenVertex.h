// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/// @file GenVertex.h
/// @brief Definition of \b class GenVertex
//
#ifndef HEPMC_GENVERTEX_H
#define HEPMC_GENVERTEX_H

#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Data/GenVertexData.h"
#include "HepMC/FourVector.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC {


/** @brief Type of iteration. Ssed by backward-compatibility interface */
#ifndef HEPMC_NO_DEPRECATED
enum IteratorRange { parents, children, family, ancestors, descendants, relatives };
#endif


class GenEvent;


/// Stores vertex-related information
class GenVertex {

friend class GenEvent;
friend class SmartPointer<GenVertex>;

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenVertex( const FourVector& position = FourVector::ZERO_VECTOR() );

    /** @brief Constructor based on vertex data */
    GenVertex( const GenVertexData &data );

//
// Functions
//
public:
    /** @brief Print information about the vertex */
    void print( std::ostream& ostr = std::cout ) const;

    /** @brief Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_event); }

protected:
    /** @brief Print information about the vertex in event-listing format */
    void print_event_listing( std::ostream& ostr = std::cout ) const;


//
// Accessors
//
public:
    GenEvent*            parent_event() const { return m_event; } //!< Get parent event
    int                  id()           const { return m_id;    } //!< Get vertex id
    const GenVertexData& data()         const { return m_data;  } //!< Get vertex data

    void add_particle_in ( const GenParticlePtr &p ); //!< Add incoming particle
    void add_particle_out( const GenParticlePtr &p ); //!< Add outgoing particle

    const vector<GenParticlePtr>& particles_in()  const { return m_particles_in;  } //!< Get/set list of incoming particles
    const vector<GenParticlePtr>& particles_out() const { return m_particles_out; } //!< Get/set list of outgoing particles

    /** @brief Get position
     *
     *  Returns position of this vertex. If position is not set, searches
     *  production vertices of ancestors to find position.
     *  Returns FourVector(0,0,0,0) if no position information found.
     */
    const FourVector& position() const;
    void              set_position(const FourVector& new_pos); //!< Set position

    /** @brief Get barcode
     *
     *  Currently barcode = id
     *  @todo Write proper barcode once we decide how it should look like
     */
    int barcode() const { return m_id; }


    /// @name Deprecated functionality
    //@{

    #ifndef HEPMC_NO_DEPRECATED

    /// Add incoming particle by raw pointer
    /// @deprecated Use GenVertex::add_particle_in( const GenParticlePtr &p ) instead
    HEPMC_DEPRECATED("Use GenParticlePtr instead of GenParticle*")
    void add_particle_in ( GenParticle *p ) { add_particle_in (GenParticlePtr(p)); }

    /// Add outgoing particle by raw pointer
    /// @deprecated Use GenVertex::add_particle_out( const GenParticlePtr &p ) instead
    HEPMC_DEPRECATED("Use GenParticlePtr instead of GenParticle*")
    void add_particle_out( GenParticle *p ) { add_particle_out(GenParticlePtr(p)); }

    /// Define iterator by typedef
    typedef std::vector<GenParticlePtr>::const_iterator particles_in_const_iterator;
    /// Define iterator by typedef
    typedef std::vector<GenParticlePtr>::const_iterator particles_out_const_iterator;
    /// Define iterator by typedef
    typedef std::vector<GenParticlePtr>::iterator       particle_iterator;

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles_in() instead")
    particles_in_const_iterator  particles_in_const_begin()  const { return m_particles_in.begin();  } //!< @deprecated Backward compatibility iterators

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles_in() instead")
    particles_in_const_iterator  particles_in_const_end()    const { return m_particles_in.end();    } //!< @deprecated Backward compatibility iterators

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles_out() instead")
    particles_out_const_iterator particles_out_const_begin() const { return m_particles_out.begin(); } //!< @deprecated Backward compatibility iterators

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles_out() instead")
    particles_out_const_iterator particles_out_const_end()   const { return m_particles_out.end();   }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Use particles_in/out() functions instead")
    particle_iterator particles_begin(IteratorRange range) {
        if (range == parents) return m_particles_in.begin();
        if (range == children) return m_particles_out.begin();
        throw Exception("GenVertex::particles_begin: Only 'parents' and 'children' ranges allowed.");
    }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED() particle_iterator particles_end(IteratorRange range) {
        if (range == parents) return m_particles_in.end();
        if (range == children) return m_particles_out.end();
        throw Exception("GenVertex::particles_begin: Only 'parents' and 'children' ranges allowed.");
    }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use particles_in().size() instead")
    int particles_in_size()  const { return m_particles_in.size(); }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use particles_out().size() instead")
    int particles_out_size() const { return m_particles_out.size(); }

    #endif

    //@}


private:

    /// @name Fields
    //@{
    GenEvent      *m_event; //!< Parent event
    int            m_id;    //!< Vertex id
    GenVertexData  m_data;  //!< Vertex data

    vector<GenParticlePtr>  m_particles_in;  //!< Incoming particle list
    vector<GenParticlePtr>  m_particles_out; //!< Outgoing particle list
    weak_ptr<GenVertex>     m_this;          //!< Pointer to shared pointer managing this vertex
    //@}

};


} // namespace HepMC

#endif
