// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenEvent.h
 *  @brief Definition of \b class GenEvent
 */
#ifndef  HEPMC_GENEVENT_H
#define  HEPMC_GENEVENT_H

#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Units.h"
#include "HepMC/GenHeavyIon.h"
#include "HepMC/GenPdfInfo.h"
#include "HepMC/GenCrossSection.h"

#include <iostream>
#include <vector>
using std::vector;
using std::pair;
using std::make_pair;

namespace HepMC {


struct GenEventData;


/// @brief Stores event-related information
///
/// Manages event-related information.
/// Contains lists of GenParticle and GenVertex objects
class GenEvent {
public:

    /// Default constructor
    GenEvent(Units::MomentumUnit momentum_unit, Units::LengthUnit length_unit);


    /// @name Print functions
    //@{

    /// @brief Print event
    void print( std::ostream& ostr = std::cout ) const;

    /// @brief Dump the whole content of the event memory. Useful for debugging
    /// @too Dump to where, in what format? Is this really a core API feature?
    void dump() const;

    /// Get printout precision. Default is 2
    int  print_precision() const { return m_print_precision; }
    /// Set printout precision
    void set_print_precision(int precision) { m_print_precision = precision; }

    //@}


    /// @name Content allocation
    //@{

    /** @brief Add particle */
    void add_particle( const GenParticlePtr &p );

    /** @brief Add vertex */
    void add_vertex( const GenVertexPtr &v );

    /** @brief Add whole tree in topological order
     *
     *  This function will find the beam particles (particles
     *  that have no production vertices or their production vertices
     *  have no particles) and will add the whole decay tree starting from
     *  these particles.
     *
     *  @note Any particles on this list that do not belong to the tree
     *        will be ignored.
     */
    void add_tree( const vector<GenParticlePtr> &particles );

    /** @brief Reserve memory for particles and vertices
     *
     *  Helps optimize event creation when size of the event is known beforehand
     */
    void reserve(unsigned int particles, unsigned int vertices = 0);

    /** @brief Change event units
     *
     *  Converts event from current units to new ones
     */
    void set_units( Units::MomentumUnit new_momentum_unit, Units::LengthUnit new_length_unit);

    /** @brief Remove contents of this event */
    void clear();

    //@}


    /// @name Accessors
    //@{

    /// @todo Remove printing from the GenEvent itself -- we should make lightweight external functions for this, to not hard-code formatting

    /// Get event number
    int  event_number() const { return m_event_number; }
    /// Set event number
    void set_event_number(int num) { m_event_number = num; }

    /// Get number of particles
    /// @todo Not particles_size()? size() is the standard name. Or require particles().size() which is as efficient.
    unsigned int particles_count() const { return m_particles.size(); }
    /// Get number of vertices
    /// @todo Not vertices_size()? size() is the standard name. Or require vertices().size() which is as efficient.
    unsigned int vertices_count() const { return m_vertices.size(); }

    /// Get/set list of particles
    const vector<GenParticlePtr>& particles() const { return m_particles; }
    /// Get/set list of vertices
    const vector<GenVertexPtr>& vertices() const { return m_vertices; }

    /// Get momentum unit
    const Units::MomentumUnit& momentum_unit() const { return m_momentum_unit; }
    /// Get length unit
    const Units::LengthUnit& length_unit() const { return m_length_unit; }

    /// Get heavy ion generator additional information
    const GenHeavyIon* heavy_ion() const { return m_heavy_ion.get(); }
    /// Set heavy ion generator additional information
    void set_heavy_ion(GenHeavyIonPtr hi) { m_heavy_ion = hi; }

    /// Get PDF information
    const GenPdfInfo* pdf_info() const { return m_pdf_info.get(); }
    /// Set PDF information
    void set_pdf_info(GenPdfInfoPtr pi) { m_pdf_info = pi; }

    /// Get cross-section information
    const GenCrossSection* cross_section() const { return m_cross_section.get(); }
    /// Set cross-section information
    void set_cross_section(GenCrossSectionPtr cs) { m_cross_section = cs; }

    /// Test to see if we have two valid beam particles
    bool valid_beam_particles() const { return (bool)m_beam_particle_1 && (bool)m_beam_particle_2; }
    /// Get incoming beam particles
    pair<GenParticlePtr,GenParticlePtr> beam_particles() const { return make_pair(m_beam_particle_1,m_beam_particle_2); }
    /// Set incoming beam particles
    void set_beam_particles(const GenParticlePtr& p1, const GenParticlePtr& p2) { m_beam_particle_1 = p1; m_beam_particle_2 = p2; }
    /// Set incoming beam particles
    void set_beam_particles(const pair<GenParticlePtr,GenParticlePtr>& p) { m_beam_particle_1 = p.first; m_beam_particle_2 = p.second; }

    //@}


    /// @name Deprecated functionality
    //@{

    #ifndef HEPMC_NO_DEPRECATED

    /// Add particle by raw pointer
    /// @deprecated Use GenEvent::add_particle( const GenParticlePtr& ) instead
    HEPMC_DEPRECATED("Use GenParticlePtr instead of GenParticle*")
    void add_particle( GenParticle *p );

    /// Add vertex by raw pointer
    /// @deprecated Use GenEvent::add_vertex( const GenVertexPtr& ) instead
    HEPMC_DEPRECATED("Use GenVertexPtr instead of GenVertex*")
    void add_vertex  ( GenVertex *v );

    /// Set heavy ion generator additional information by raw pointer
    /// @deprecated Use GenEvent::set_heavy_ion( GenHeavyIonPtr hi) instead
    HEPMC_DEPRECATED("Use GenHeavyIonPtr instead of GenHeavyIon*")
    void set_heavy_ion(GenHeavyIon *hi);

    /// Set PDF information by raw pointer
    /// @deprecated Use GenEvent::set_pdf_info( GenPdfInfoPtr pi) instead
    HEPMC_DEPRECATED("Use GenPdfInfoPtr instead of GenPdfInfo*")
    void set_pdf_info(GenPdfInfo *pi);

    /// Set cross-section information by raw pointer
    /// @deprecated Use GenEvent::set_cross_section( GenCrossSectionPtr cs) instead
    HEPMC_DEPRECATED("Use GenCrossSectionPtr instead of GenCrossSection*")
    void set_cross_section(GenCrossSection *cs);

    /// @deprecated Backward compatibility iterators
    typedef std::vector<GenParticlePtr>::iterator       particle_iterator;
    /// @deprecated Backward compatibility iterators
    typedef std::vector<GenParticlePtr>::const_iterator particle_const_iterator;

    /// @deprecated Backward compatibility iterators
    typedef std::vector<GenVertexPtr>::iterator         vertex_iterator;
    /// @deprecated Backward compatibility iterators
    typedef std::vector<GenVertexPtr>::const_iterator   vertex_const_iterator;

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead")
    particle_iterator       particles_begin()       { return m_particles.begin(); }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead")
    particle_iterator       particles_end()         { return m_particles.end();   }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead")
    particle_const_iterator particles_begin() const { return m_particles.begin(); }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead")
    particle_const_iterator particles_end()   const { return m_particles.end();   }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")
    vertex_iterator         vertices_begin()        { return m_vertices.begin();  }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")
    vertex_iterator         vertices_end()          { return m_vertices.end();    }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")
    vertex_const_iterator   vertices_begin()  const { return m_vertices.begin();  }

    /// @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")
    vertex_const_iterator   vertices_end()    const { return m_vertices.end();    }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use particles().size() instead")
    int  particles_size()  const { return m_particles.size();  }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use particles().empty() instead")
    bool particles_empty() const { return m_particles.empty(); }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use vertices().size() instead")
    int  vertices_size()   const { return m_vertices.size();   }

    /// @deprecated Backward compatibility
    HEPMC_DEPRECATED("Use vertices().empty() instead")
    bool vertices_empty()  const { return m_vertices.empty();  }

    #endif

    //@}


private:

    /// @name Fields
    //@{
    int                         m_event_number;    //!< Event number
    int                         m_print_precision; //!< Printout precision
    Units::MomentumUnit         m_momentum_unit;   //!< Momentum unit
    Units::LengthUnit           m_length_unit;     //!< Length unit
    GenHeavyIonPtr              m_heavy_ion;       //!< Heavy ion generator additional information
    GenPdfInfoPtr               m_pdf_info;        //!< Pdf information
    GenCrossSectionPtr          m_cross_section;   //!< Cross-section information
    GenParticlePtr              m_beam_particle_1; //!< First beam particle
    GenParticlePtr              m_beam_particle_2; //!< Second beam particle
    std::vector<GenParticlePtr> m_particles;       //!< List of particles
    std::vector<GenVertexPtr>   m_vertices;        //!< List of vertices
    //@}

};


} // namespace HepMC

#endif
