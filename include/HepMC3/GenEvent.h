#ifndef  HEPMC3_GENEVENT_H
#define  HEPMC3_GENEVENT_H
/**
 *  @file GenEvent.h
 *  @brief Definition of \b class HepMC3::GenEvent
 *
 *  @class HepMC3::GenEvent
 *  @brief Stores event-related information
 *
 *  Deleting the event will delete all particles and vertices
 *  that belong to this event
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <boost/range/iterator_range.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include "HepMC3/GenEventVersion.h"
using std::vector;

namespace HepMC3 {

class GenVertex;
class GenParticle;

class GenEvent {
//
// Constructors
//
public:
    /** Default constructor */
    GenEvent();

//
// Functions
//
public:
    /** Print information about the event and list full event tree */
    void print( std::ostream& ostr = std::cout ) const { print_version( m_last_version, ostr ); }

    /** Print information about the selected version of the event */
    void print_version( int version, std::ostream& ostr = std::cout ) const;

    /** Set printout precision. Default is 2 */
    void set_print_precision(int precision)      { m_print_precision = precision; }

    /** Get particle by barcode */
    const GenParticle& get_particle(int barcode) const { return m_versions[m_last_version].get_particle(barcode); }

    /** Get vertex by barcode */
    const GenVertex&   get_vertex(int barcode)   const { return m_versions[m_last_version].get_vertex(barcode); }

    /** Create new particle and add it to this version of event */
    GenParticle& create_particle()       { ++m_last_particle; return m_versions[m_last_version].create_particle(); }

    /** Create new vertex and add it to this version of event */
    GenVertex&   create_vertex()         { --m_last_vertex;   return m_versions[m_last_version].create_vertex(); }

    /** Delete vertex from this version of event */
    void delete_particle(GenParticle &p) { m_versions[m_last_version].delete_particle(p); }

    /** Delete vertex from this version of event */
    void delete_vertex(GenVertex &v)     { m_versions[m_last_version].delete_vertex(v); }

    /** Create new version
     *  Sets new version as current version
     */
    void create_new_version(const char *name);
//
// Accessors
//
public:
    int  event_number()                       const { return m_event_number;   } //!< Get event number
    void set_event_number(int no)                   { m_event_number = no;     } //!< Set event number

    int  particles_count()                    const { return  m_last_particle; } //!< Get number of particles
    int  vertices_count()                     const { return -m_last_vertex;   } //!< Get number of vertices
    int  last_version()                       const { return  m_last_version;  } //!< Get last version

    const vector<GenEventVersion>& versions() const { return  m_versions;      } //!< Get list of versions
//
// Fields
//
private:
    int m_event_number;       //!< Event number
    int m_last_particle;      //!< Barcode of the last particle in the event
    int m_last_vertex;        //!< Barcode of the last vertex in the event
    int m_last_version;       //!< Index of the last version of the event
    int m_print_precision;    //!< Printout precision

    vector<GenEventVersion> m_versions; //!< version list
};

} // namespace HepMC3

#endif
