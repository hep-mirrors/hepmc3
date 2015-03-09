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
#include <map>
using std::vector;
using std::pair;
using std::map;
using std::make_pair;

#if __cplusplus >= 201103L
using std::dynamic_pointer_cast;
#else
using boost::dynamic_pointer_cast;
#endif

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

    /// Default constructor
    GenEvent(Units::MomentumUnit momentum_unit = Units::GEV, Units::LengthUnit length_unit = Units::MM);

    /// @name Print functions
    //@{

    /// @brief Print event
    void print( std::ostream& ostr = std::cout ) const;

    /// @brief Dump the whole content of the event memory. Useful for debugging
    /// @todo Dump to where, in what format? Is this really a core API feature?
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
    const GenHeavyIonPtr heavy_ion() const { return attribute<GenHeavyIon>("GenHeavyIon"); }
    /// Set heavy ion generator additional information
    void set_heavy_ion(const GenHeavyIonPtr &hi) { add_attribute("GenHeavyIon",hi); }

    /// Get PDF information
    const GenPdfInfoPtr pdf_info() const { return attribute<GenPdfInfo>("GenPdfInfo"); }
    /// Set PDF information
    void set_pdf_info(const GenPdfInfoPtr &pi) { add_attribute("GenPdfInfo",pi); }

    /// Get cross-section information
    const GenCrossSectionPtr cross_section() const { return attribute<GenCrossSection>("GenCrossSection"); }
    /// Set cross-section information
    void set_cross_section(const GenCrossSectionPtr &cs) { add_attribute("GenCrossSection",cs); }

    /// Test to see if we have two valid beam particles
    bool valid_beam_particles() const { return (bool)m_beam_particle_1 && (bool)m_beam_particle_2; }
    /// Get incoming beam particles
    pair<GenParticlePtr,GenParticlePtr> beam_particles() const { return make_pair(m_beam_particle_1,m_beam_particle_2); }
    /// Set incoming beam particles
    void set_beam_particles(const GenParticlePtr& p1, const GenParticlePtr& p2) { m_beam_particle_1 = p1; m_beam_particle_2 = p2; }
    /// Set incoming beam particles
    void set_beam_particles(const pair<GenParticlePtr,GenParticlePtr>& p) { m_beam_particle_1 = p.first; m_beam_particle_2 = p.second; }

    /** @brief Add event attribute to event
     *
     *  This will overwrite existing attribute if an attribute
     *  with the same name is present
     */
    void add_attribute(const string &name, const shared_ptr<Attribute> &att,
		       int id = 0) {
      if ( att ) m_attributes[name][id] = att;
    }

    /// Remove attribute
    void remove_attribute(const string &name, int id = 0);

    /// Get attribute of type T
    template<class T>
    shared_ptr<T> attribute(const string &name, int id = 0) const;

    /// Get list of attributes
    const map< string, map<int, shared_ptr<Attribute> > >& attributes() const { return m_attributes; }

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

  // methods to fill GenEventData and to read it back

  void write_data(GenEventData &data) const;
  void read_data(const GenEventData &data);

  
#ifdef HEPMC_ROOTIO

  void Streamer(TBuffer &b);

#endif
  
private:

    /// @name Fields
    //@{
    int                         m_event_number;    //!< Event number
    int                         m_print_precision; //!< Printout precision
    Units::MomentumUnit         m_momentum_unit;   //!< Momentum unit
    Units::LengthUnit           m_length_unit;     //!< Length unit
    GenParticlePtr              m_beam_particle_1; //!< First beam particle
    GenParticlePtr              m_beam_particle_2; //!< Second beam particle
    std::vector<GenParticlePtr> m_particles;       //!< List of particles
    std::vector<GenVertexPtr>   m_vertices;        //!< List of vertices

    /** @brief Map of event, particle and vertex attributes
     *
     *  Keys are name and id (0 = event, <0 = vertex, >0 = particle)
     */
    mutable map< string, map<int, shared_ptr<Attribute> > > m_attributes;
    //@}

};

//
// Template methods
//

template<class T>
shared_ptr<T> GenEvent::attribute(const string &name, int id) const {

    map< string, map<int, shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if( i1 == m_attributes.end() ) return shared_ptr<T>();

    map<int, shared_ptr<Attribute> >::iterator i2 = i1->second.find(id);
    if( i2 == i1->second.end() ) return shared_ptr<T>();

    if( !i2->second->is_parsed() ) {

        shared_ptr<T> att = make_shared<T>();
        att->from_string(i2->second->unparsed_string());

        // update map with new pointer
        i2->second = att;

        return att;
    }
    else return dynamic_pointer_cast<T>(i2->second);
}

} // namespace HepMC

#endif
