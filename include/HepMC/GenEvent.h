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
#include "HepMC/Attribute.h"
#include "HepMC/GenHeavyIon.h"
#include "HepMC/GenPdfInfo.h"
#include "HepMC/GenCrossSection.h"

#include <iostream>
#include <vector>
#include <map>
using std::vector;
using std::map;
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

    const map< string, map<int, shared_ptr<Attribute> > >& attributes() const { return m_attributes; } //!< Get list of attributes

    const Units::MomentumUnit& momentum_unit() const { return m_momentum_unit;        } //!< Get momentum unit
    const Units::LengthUnit&   length_unit()   const { return m_length_unit;          } //!< Get length unit

    GenHeavyIonPtr     heavy_ion()                             { return attribute<GenHeavyIon>("GenHeavyIon"); } //!< Get heavy ion generator additional information
    void               set_heavy_ion(const GenHeavyIonPtr &hi) { add_attribute("GenHeavyIon",hi);              } //!< Set heavy ion generator additional information

    GenPdfInfoPtr      pdf_info()                              { return attribute<GenPdfInfo>("GenPdfInfo"); } //!< Get pdf information
    void               set_pdf_info(const GenPdfInfoPtr &pi)   { add_attribute("GenPdfInfo",pi);             } //!< Set pdf information

    GenCrossSectionPtr cross_section()                                 { return attribute<GenCrossSection>("GenCrossSection"); } //!< Get cross-section information
    void               set_cross_section(const GenCrossSectionPtr &cs) { add_attribute("GenCrossSection",cs);                  } //!< Set cross-section information

    /** @brief Add event attribute to event
     *
     *  This will overwrite existing attribute if an attribute
     *  with the same name is present
     */
    void add_attribute(const string &name, const shared_ptr<Attribute> &att);

    /** @brief Remove attribute by name */
    void remove_attribute(const string &name);

    /** @brief Get attribute of type T by name */
    template<class T>
    shared_ptr<T> attribute(const string &name);

    bool                                valid_beam_particles()                                                 const { return (bool)m_beam_particle_1 && (bool)m_beam_particle_2; } //!< Test to see if we have two valid beam particles
    pair<GenParticlePtr,GenParticlePtr> beam_particles()                                                       const { return make_pair(m_beam_particle_1,m_beam_particle_2);     } //!< Get incoming beam particles
    void                                set_beam_particles(const GenParticlePtr &p1, const GenParticlePtr &p2)       { m_beam_particle_1 = p1;      m_beam_particle_2 = p2;       } //!< Set incoming beam particles
    void                                set_beam_particles(const pair<GenParticlePtr,GenParticlePtr> &p)             { m_beam_particle_1 = p.first; m_beam_particle_2 = p.second; } //!< Set incoming beam particles
//
// Deprecated functionality
//
public:
    __attribute__((deprecated("Use GenParticlePtr instead of GenParticle*"))) void add_particle( GenParticle *p ); //!< Add particle by raw pointer @deprecated Use GenEvent::add_particle( const GenParticlePtr& ) instead
    __attribute__((deprecated("Use GenVertexPtr instead of GenVertex*")))     void add_vertex  ( GenVertex *v );   //!< Add vertex by raw pointer   @deprecated Use GenEvent::add_vertex( const GenVertexPtr& ) instead

    __attribute__((deprecated("Use GenHeavyIonPtr instead of GenHeavyIon*")))         void set_heavy_ion(GenHeavyIon *hi);         //!< Set heavy ion generator additional information by raw pointer @deprecated Use GenEvent::set_heavy_ion( GenHeavyIonPtr hi);         instead
    __attribute__((deprecated("Use GenPdfInfoPtr instead of GenPdfInfo*")))           void set_pdf_info(GenPdfInfo *pi);           //!< Set pdf information by raw pointer                            @deprecated Use GenEvent::set_pdf_info( GenPdfInfoPtr pi);           instead
    __attribute__((deprecated("Use GenCrossSectionPtr instead of GenCrossSection*"))) void set_cross_section(GenCrossSection *cs); //!< Set cross-section information by raw pointer                  @deprecated Use GenEvent::set_cross_section( GenCrossSectionPtr cs); instead

    typedef std::vector<GenParticlePtr>::iterator       particle_iterator;       //!< @deprecated Backward compatibility iterators
    typedef std::vector<GenParticlePtr>::const_iterator particle_const_iterator; //!< @deprecated Backward compatibility iterators

    typedef std::vector<GenVertexPtr>::iterator         vertex_iterator;         //!< @deprecated Backward compatibility iterators
    typedef std::vector<GenVertexPtr>::const_iterator   vertex_const_iterator;   //!< @deprecated Backward compatibility iterators

    __attribute__((deprecated("Iterate over std container particles() instead"))) particle_iterator       particles_begin()       { return m_particles.begin(); } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container particles() instead"))) particle_iterator       particles_end()         { return m_particles.end();   } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container particles() instead"))) particle_const_iterator particles_begin() const { return m_particles.begin(); } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container particles() instead"))) particle_const_iterator particles_end()   const { return m_particles.end();   } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container vertices() instead")))  vertex_iterator         vertices_begin()        { return m_vertices.begin();  } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container vertices() instead")))  vertex_iterator         vertices_end()          { return m_vertices.end();    } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container vertices() instead")))  vertex_const_iterator   vertices_begin()  const { return m_vertices.begin();  } //!< @deprecated Backward compatibility iterators
    __attribute__((deprecated("Iterate over std container vertices() instead")))  vertex_const_iterator   vertices_end()    const { return m_vertices.end();    } //!< @deprecated Backward compatibility iterators

    __attribute__((deprecated("Use particles().size() instead")))  int  particles_size()  const { return m_particles.size();  } //!< @deprecated backward compatibility
    __attribute__((deprecated("Use particles().empty() instead"))) bool particles_empty() const { return m_particles.empty(); } //!< @deprecated backward compatibility
    __attribute__((deprecated("Use vertices().size() instead")))   int  vertices_size()   const { return m_vertices.size();   } //!< @deprecated backward compatibility
    __attribute__((deprecated("Use vertices().empty() instead")))  bool vertices_empty()  const { return m_vertices.empty();  } //!< @deprecated backward compatibility

//
// Fields
//
private:
    int                         m_event_number;    //!< Event number
    int                         m_print_precision; //!< Printout precision
    Units::MomentumUnit         m_momentum_unit;   //!< Momentum unit
    Units::LengthUnit           m_length_unit;     //!< Length unit
    GenParticlePtr              m_beam_particle_1; //!< First beam particle
    GenParticlePtr              m_beam_particle_2; //!< Second beam particle
    vector<GenParticlePtr>      m_particles;       //!< List of particles
    vector<GenVertexPtr>        m_vertices;        //!< List of vertices

    /** @brief Map of event, particle and vertex attributes
     *
     *  Keys are name and id (0 = event, <0 = vertex, >0 = particle)
     */
    map< string, map<int, shared_ptr<Attribute> > > m_attributes;
};

//
// Template methods
//

template<class T>
shared_ptr<T> GenEvent::attribute(const string &name) {

    map< string, map<int, shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if( i1 == m_attributes.end() ) return shared_ptr<T>();

    map<int, shared_ptr<Attribute> >::iterator i2 = i1->second.find(0);
    if( i2 == i1->second.end() ) return shared_ptr<T>();

    if( !i2->second->is_parsed() ) {

        shared_ptr<T> att = make_shared<T>();
        att->parse_attribute_container(i2->second->attribute_container());
        i2->second = att;

        return att;
    }
    else return std::dynamic_pointer_cast<T>(i2->second);
}

} // namespace HepMC

#endif
