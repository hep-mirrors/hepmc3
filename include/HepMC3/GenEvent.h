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
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/DataList.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

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
    /** Print current version of the event */
    void print( std::ostream& ostr = std::cout ) const { print_version( m_versions.size()-1, ostr ); }

    /** Print selected version of the event */
    void print_version( unsigned int version, std::ostream& ostr = std::cout ) const;

    void dump() const;

    /** Get particle by barcode */
    GenParticle& get_particle(int barcode) const;

    /** Get vertex by barcode */
    GenVertex&   get_vertex(int barcode) const;

    /** Create new particle and add it to this version of event */
    GenParticle& new_particle();

    /** Create new vertex and add it to this version of event */
    GenVertex&   new_vertex();

    /** Delete vertex from this version of event */
    void delete_particle(GenParticle &p);

    /** Delete vertex from this version of event */
    void delete_vertex(GenVertex &v);

    /** Create new version */
    void new_version(const std::string name);

    /** Record change of a particle
     *  Create a copy if a new version has been created and this particle
     *  has not yet been added to the new version
     *  @return barcode of the particle that has to be changed
     */
    int record_change(GenParticle& p);

    /** Record change of a vertex
     *  Create a copy if a new version has been created and this vertex
     *  has not yet been added to the new version
     *  @return barcode of the vertex that has to be changed
     */
    int record_change(GenVertex& p);
//
// Accessors
//
public:
    void set_print_precision(int precision)      { m_print_precision = precision; } //!< Set printout precision. Default is 2

    int  event_number()                    const { return m_event_number;   } //!< Get event number
    void set_event_number(int no)                { m_event_number = no;     } //!< Set event number

    unsigned int  particles_count()        const { return  m_particles.size();  } //!< Get number of particles
    unsigned int  vertices_count()         const { return  m_vertices.size();   } //!< Get number of vertices
    unsigned int  last_version()           const { return  m_versions.size()-1; } //!< Get last version number
//
// Fields
//
private:
    int                     m_event_number;    //!< Event number
    int                     m_print_precision; //!< Printout precision
    vector<std::string>     m_versions;        //!< List of version names
    DataList<GenParticle,8> m_particles;       //!< List of particles
    DataList<GenVertex,8>   m_vertices;        //!< List of vertices
};

} // namespace HepMC3

#endif
