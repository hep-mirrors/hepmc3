// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file GenEvent.h
/// @brief Definition of \b class GenEvent
///
#ifndef  HEPMC_GENEVENT_H
#define  HEPMC_GENEVENT_H

#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Units.h"
#include "HepMC/GenWeights.h"
#include "HepMC/GenHeavyIon.h"
#include "HepMC/GenPdfInfo.h"
#include "HepMC/GenCrossSection.h"

#include <iostream>
#include <vector>
#include <map>

#ifdef HEPMC_ROOTIO
#include "TBuffer.h"
#include "TClass.h"
#endif

namespace HepMC {


struct GenEventData;


/// @brief Stores event-related information
///
/// Manages event-related information.
/// Contains lists of GenParticle and GenVertex objects
class GenEvent {
public:

    /// @brief Default constructor
    GenEvent(Units::MomentumUnit momentum_unit=Units::GEV, Units::LengthUnit length_unit=Units::MM);


    /// @name Particle and vertex read access
    //@{

    /// @brief Get/set list of particles
    const std::vector<GenParticlePtr>& particles() const { return m_particles; }
    /// @brief Get/set list of vertices
    const std::vector<GenVertexPtr>& vertices() const { return m_vertices; }

    //@}


    /// @name Auxiliary info and event metadata
    //@{

    /// @brief Get event number
    int  event_number() const { return m_event_number; }
    /// @brief Set event number
    void set_event_number(int num) { m_event_number = num; }

    /// Get event weights
    const GenWeights& weights() const { return m_weights; }
    /// Get event weights (non-const)
    GenWeights& weights() { return m_weights; }
    /// Get canonical event weight (might not exist...)
    double weight() const { return weights()[0]; }

    /// @brief Get momentum unit
    const Units::MomentumUnit& momentum_unit() const { return m_momentum_unit; }
    /// @brief Get length unit
    const Units::LengthUnit& length_unit() const { return m_length_unit; }
    /// @brief Change event units
    /// Converts event from current units to new ones
    void set_units( Units::MomentumUnit new_momentum_unit, Units::LengthUnit new_length_unit);

    /// @brief Get heavy ion generator additional information
    const GenHeavyIonPtr heavy_ion() const { return attribute<GenHeavyIon>("GenHeavyIon"); }
    /// @brief Set heavy ion generator additional information
    void set_heavy_ion(const GenHeavyIonPtr &hi) { add_attribute("GenHeavyIon",hi); }

    /// @brief Get PDF information
    const GenPdfInfoPtr pdf_info() const { return attribute<GenPdfInfo>("GenPdfInfo"); }
    /// @brief Set PDF information
    void set_pdf_info(const GenPdfInfoPtr &pi) { add_attribute("GenPdfInfo",pi); }

    /// @brief Get cross-section information
    const GenCrossSectionPtr cross_section() const { return attribute<GenCrossSection>("GenCrossSection"); }
    /// @brief Set cross-section information
    void set_cross_section(const GenCrossSectionPtr &cs) { add_attribute("GenCrossSection",cs); }

    //@}


    /// @name Beam particles
    //@{

    /// @brief Test to see if we have two valid beam particles
    bool valid_beam_particles() const { return (bool)m_beam_particle_1 && (bool)m_beam_particle_2; }

    /// @brief Get incoming beam particles
    std::pair<GenParticlePtr,GenParticlePtr> beam_particles() const { return std::make_pair(m_beam_particle_1,m_beam_particle_2); }

    /// @brief Set incoming beam particles
    /// @todo Set/require status = 4 at the same time?
    void set_beam_particles(const GenParticlePtr& p1, const GenParticlePtr& p2) { m_beam_particle_1 = p1; m_beam_particle_2 = p2; }

    /// @brief Set incoming beam particles
    /// @todo Set/require status = 4 at the same time?
    void set_beam_particles(const std::pair<GenParticlePtr,GenParticlePtr>& p) { m_beam_particle_1 = p.first; m_beam_particle_2 = p.second; }

    //@}


    /// @name Attributes
    //@{

    /// @brief Add event attribute to event
    ///
    /// This will overwrite existing attribute if an attribute
    /// with the same name is present
    void add_attribute(const string &name, const shared_ptr<Attribute> &att, int id = 0) {
      if ( att ) m_attributes[name][id] = att;
    }

    /// @brief Remove attribute
    void remove_attribute(const string &name, int id = 0);

    /// @brief Get attribute of type T
    template<class T>
    shared_ptr<T> attribute(const string &name, int id = 0) const;

    /// @brief Get list of attributes
    const std::map< string, std::map<int, shared_ptr<Attribute> > >& attributes() const { return m_attributes; }

    //@}


    /// @name Particle and vertex modification
    //@{

    /// @brief Add particle
    void add_particle( const GenParticlePtr &p );

    /// @brief Add vertex
    void add_vertex( const GenVertexPtr &v );

    /// @brief Remove particle
    ///
    /// This will remove whole sub-tree starting from this particle
    /// and will remove production vertex of this particle if this vertex
    /// has no more outgoing particles
    /// @todo Rejoining vertices. Also: what to do if deleting beam particle?
    void remove_particle( const GenParticlePtr &v );

    /// @brief Remove vertex
    ///
    /// This will remove all sub-trees of all outgoing particles of this vertex
    /// @todo Optimize. Currently each particle/vertex is erased separately
    void remove_vertex( const GenVertexPtr &v );

    /// @brief Add whole tree in topological order
    ///
    /// This function will find the beam particles (particles
    /// that have no production vertices or their production vertices
    /// have no particles) and will add the whole decay tree starting from
    /// these particles.
    ///
    /// @note Any particles on this list that do not belong to the tree
    ///       will be ignored.
    void add_tree( const std::vector<GenParticlePtr> &particles );

    /// @brief Reserve memory for particles and vertices
    ///
    /// Helps optimize event creation when size of the event is known beforehand
    void reserve(unsigned int particles, unsigned int vertices = 0);

    /// @brief Remove contents of this event
    void clear();

    //@}


    /// @name Deprecated functionality
    //@{

    #ifndef HEPMC_NO_DEPRECATED

    /// @brief Add particle by raw pointer
    /// @deprecated Use GenEvent::add_particle( const GenParticlePtr& ) instead
    HEPMC_DEPRECATED("Use GenParticlePtr instead of GenParticle*")
    void add_particle( GenParticle *p );

    /// @brief Add vertex by raw pointer
    /// @deprecated Use GenEvent::add_vertex( const GenVertexPtr& ) instead
    HEPMC_DEPRECATED("Use GenVertexPtr instead of GenVertex*")
    void add_vertex  ( GenVertex *v );

    /// @brief Set heavy ion generator additional information by raw pointer
    /// @deprecated Use GenEvent::set_heavy_ion( GenHeavyIonPtr hi) instead
    HEPMC_DEPRECATED("Use GenHeavyIonPtr instead of GenHeavyIon*")
    void set_heavy_ion(GenHeavyIon *hi);

    /// @brief Set PDF information by raw pointer
    /// @deprecated Use GenEvent::set_pdf_info( GenPdfInfoPtr pi) instead
    HEPMC_DEPRECATED("Use GenPdfInfoPtr instead of GenPdfInfo*")
    void set_pdf_info(GenPdfInfo *pi);

    /// @brief Set cross-section information by raw pointer
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


    /// @name Methods to fill GenEventData and to read it back
    //@{

    /// @brief Fill GenEventData object
    void write_data(GenEventData &data) const;

    /// @brief Fill GenEvent based on GenEventData
    void read_data(const GenEventData &data);

    #ifdef HEPMC_ROOTIO
    /// @brief ROOT I/O streamer
    void Streamer(TBuffer &b);
    #endif

    //@}


private:

    /// @name Fields
    //@{

    /// List of particles
    std::vector<GenParticlePtr> m_particles;
    /// List of vertices
    std::vector<GenVertexPtr> m_vertices;

    /// Event number
    /// @todo Move to attributes?
    int m_event_number;

    /// Event weights
    /// @todo Move to attributes?
    GenWeights m_weights;

    /// Momentum unit
    /// @todo Move to attributes?
    Units::MomentumUnit m_momentum_unit;
    /// Length unit
    /// @todo Move to attributes?
    Units::LengthUnit m_length_unit;

    /// First beam particle
    /// @todo Replace with search for status = 4?
    GenParticlePtr m_beam_particle_1;
    /// Second beam particle
    /// @todo Replace with search for status = 4?
    GenParticlePtr m_beam_particle_2;

    /// @brief Map of event, particle and vertex attributes
    ///
    /// Keys are name and ID (0 = event, <0 = vertex, >0 = particle)
    mutable std::map< string, std::map<int, shared_ptr<Attribute> > > m_attributes;

    //@}

};



//
// Template methods
//

template<class T>
shared_ptr<T> GenEvent::attribute(const string &name, int id) const {

    std::map< string, std::map<int, shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if (i1 == m_attributes.end() ) return shared_ptr<T>();

    std::map<int, shared_ptr<Attribute> >::iterator i2 = i1->second.find(id);
    if (i2 == i1->second.end() ) return shared_ptr<T>();

    if (!i2->second->is_parsed() ) {

    shared_ptr<T> att = make_shared<T>();
    if ( att->from_string(i2->second->unparsed_string()) && att->init(*this) ) {
        // update map with new pointer
        i2->second = att;
        return att;
    } else
        return shared_ptr<T>();
    }
    else return dynamic_pointer_cast<T>(i2->second);
}


} // namespace HepMC

#endif
