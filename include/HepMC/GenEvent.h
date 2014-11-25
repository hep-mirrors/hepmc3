// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_GENEVENT_H
#define  HEPMC_GENEVENT_H
/**
 *  @file GenEvent.h
 *  @brief Definition of \b class GenEvent
 *
 *  @class HepMC::GenEvent
 *  @brief Stores event-related information
 *
 *  Manages event-related information.
 *  Contains lists of GenParticle and GenVertex objects
 *
 */
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


class GenEvent {

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenEvent(Units::MomentumUnit momentum_unit, Units::LengthUnit length_unit);

//
// Functions
//
public:
    /** @brief Print event */
    void print( std::ostream& ostr = std::cout ) const;

    /** @brief Dump the whole content of the event memory. Useful for debugging */
    void dump() const;

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

//
// Accessors
//
public:
    int  print_precision()                     const { return m_print_precision;      } //!< Get printout precision. Default is 2
    void set_print_precision(int precision)          { m_print_precision = precision; } //!< Set printout precision. Default is 2

    int  event_number()                        const { return m_event_number;         } //!< Get event number
    void set_event_number(int no)                    { m_event_number = no;           } //!< Set event number

    unsigned int particles_count()             const { return m_particles.size();     } //!< Get number of particles
    unsigned int vertices_count()              const { return m_vertices.size();      } //!< Get number of vertices

    const vector<GenParticlePtr>& particles()  const { return m_particles;            } //!< Get/set list of particles
    const vector<GenVertexPtr>&   vertices()   const { return m_vertices;             } //!< Get/set list of vertices

    const Units::MomentumUnit& momentum_unit() const { return m_momentum_unit;        } //!< Get momentum unit
    const Units::LengthUnit&   length_unit()   const { return m_length_unit;          } //!< Get length unit

    const GenHeavyIon*         heavy_ion()                      const { return m_heavy_ion.get(); } //!< Get heavy ion generator additional information
    void                       set_heavy_ion(GenHeavyIonPtr hi)       { m_heavy_ion = hi;         } //!< Set heavy ion generator additional information

    const GenPdfInfo*          pdf_info()                     const { return m_pdf_info.get(); } //!< Get pdf information
    void                       set_pdf_info(GenPdfInfoPtr pi)       { m_pdf_info = pi;         } //!< Set pdf information

    const GenCrossSection*     cross_section()                          const { return m_cross_section.get(); } //!< Get cross-section information
    void                       set_cross_section(GenCrossSectionPtr cs)       { m_cross_section = cs;         } //!< Set cross-section information

    bool                                valid_beam_particles()                                                 const { return (bool)m_beam_particle_1 && (bool)m_beam_particle_2; } //!< Test to see if we have two valid beam particles
    pair<GenParticlePtr,GenParticlePtr> beam_particles()                                                       const { return make_pair(m_beam_particle_1,m_beam_particle_2);     } //!< Get incoming beam particles
    void                                set_beam_particles(const GenParticlePtr &p1, const GenParticlePtr &p2)       { m_beam_particle_1 = p1;      m_beam_particle_2 = p2;       } //!< Set incoming beam particles
    void                                set_beam_particles(const pair<GenParticlePtr,GenParticlePtr> &p)             { m_beam_particle_1 = p.first; m_beam_particle_2 = p.second; } //!< Set incoming beam particles
//
// Deprecated functionality
//
public:
    HEPMC_DEPRECATED("Use GenParticlePtr instead of GenParticle*") void add_particle( GenParticle *p ); //!< Add particle by raw pointer @deprecated Use GenEvent::add_particle( const GenParticlePtr& ) instead
    HEPMC_DEPRECATED("Use GenVertexPtr instead of GenVertex*")     void add_vertex  ( GenVertex *v );   //!< Add vertex by raw pointer   @deprecated Use GenEvent::add_vertex( const GenVertexPtr& ) instead

    HEPMC_DEPRECATED("Use GenHeavyIonPtr instead of GenHeavyIon*")         void set_heavy_ion(GenHeavyIon *hi);         //!< Set heavy ion generator additional information by raw pointer @deprecated Use GenEvent::set_heavy_ion( GenHeavyIonPtr hi);         instead
    HEPMC_DEPRECATED("Use GenPdfInfoPtr instead of GenPdfInfo*")           void set_pdf_info(GenPdfInfo *pi);           //!< Set pdf information by raw pointer                            @deprecated Use GenEvent::set_pdf_info( GenPdfInfoPtr pi);           instead
    HEPMC_DEPRECATED("Use GenCrossSectionPtr instead of GenCrossSection*") void set_cross_section(GenCrossSection *cs); //!< Set cross-section information by raw pointer                  @deprecated Use GenEvent::set_cross_section( GenCrossSectionPtr cs); instead

    typedef std::vector<GenParticlePtr>::iterator       particle_iterator;       //!< @deprecated Backward compatibility iterators
    typedef std::vector<GenParticlePtr>::const_iterator particle_const_iterator; //!< @deprecated Backward compatibility iterators

    typedef std::vector<GenVertexPtr>::iterator         vertex_iterator;         //!< @deprecated Backward compatibility iterators
    typedef std::vector<GenVertexPtr>::const_iterator   vertex_const_iterator;   //!< @deprecated Backward compatibility iterators

    HEPMC_DEPRECATED("Iterate over std container particles() instead") particle_iterator       particles_begin()       { return m_particles.begin(); } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead") particle_iterator       particles_end()         { return m_particles.end();   } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead") particle_const_iterator particles_begin() const { return m_particles.begin(); } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container particles() instead") particle_const_iterator particles_end()   const { return m_particles.end();   } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")  vertex_iterator         vertices_begin()        { return m_vertices.begin();  } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")  vertex_iterator         vertices_end()          { return m_vertices.end();    } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")  vertex_const_iterator   vertices_begin()  const { return m_vertices.begin();  } //!< @deprecated Backward compatibility iterators
    HEPMC_DEPRECATED("Iterate over std container vertices() instead")  vertex_const_iterator   vertices_end()    const { return m_vertices.end();    } //!< @deprecated Backward compatibility iterators

    HEPMC_DEPRECATED("Use particles().size() instead")  int  particles_size()  const { return m_particles.size();  } //!< @deprecated backward compatibility
    HEPMC_DEPRECATED("Use particles().empty() instead") bool particles_empty() const { return m_particles.empty(); } //!< @deprecated backward compatibility
    HEPMC_DEPRECATED("Use vertices().size() instead")   int  vertices_size()   const { return m_vertices.size();   } //!< @deprecated backward compatibility
    HEPMC_DEPRECATED("Use vertices().empty() instead")  bool vertices_empty()  const { return m_vertices.empty();  } //!< @deprecated backward compatibility

//
// Fields
//
private:
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
};


} // namespace HepMC

#endif
