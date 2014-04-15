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
#include "HepMC3/Data/GenEventData.h"

#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

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
    void print( std::ostream& ostr = std::cout ) const { print_version( m_data.versions.size(), ostr ); }

    /** Print selected version of the event */
    void print_version( unsigned short int version, std::ostream& ostr = std::cout ) const;

    /** Dump the whole content of the event memory. Useful for debugging */
    void dump() const;

    /** Get particle by barcode */
    GenParticle& particle(int barcode) const;

    /** Get vertex by barcode */
    GenVertex&   vertex(int barcode) const;

    /** Create new particle
     *  If pointer to particle data is provided,
     *  it will be copied to the new particle
     *  @return Reference to newly created particle
     */
    GenParticle& new_particle( const GenParticleData *data = NULL );

    /** Create new vertex
     *  If pointer to vertex data is provided,
     *  it will be copied to the new vertex
     *  @return Reference to newly created vertex
     */
    GenVertex&   new_vertex( const GenVertexData *data = NULL );

    /** Delete particle */
    void delete_particle(GenParticle &p);

    /** Delete vertex */
    void delete_vertex(GenVertex &v);

    /** Create new version */
    void new_version(const std::string name);

    /** Record change of a particle
     *  Create a copy if a new version has been created and this particle
     *  has not yet been added to the new version
     */
    void record_change(GenParticle& p);

    /** Record change of a vertex
     *  Create a copy if a new version has been created and this vertex
     *  has not yet been added to the new version
     */
    void record_change(GenVertex& p);

//
// Accessors
//
public:
    void set_print_precision(int precision)       { m_print_precision = precision; } //!< Set printout precision. Default is 2

    int  event_number()                     const { return m_data.event_number;   } //!< Get event number
    void set_event_number(int no)                 { m_data.event_number = no;     } //!< Set event number

    unsigned int particles_count()          const { return  m_particles.size(); } //!< Get number of particles
    unsigned int vertices_count()           const { return  m_vertices.size();  } //!< Get number of vertices
    unsigned int last_version()             const { return  m_data.versions.size();  } //!< Get last version number

    const DataList<GenParticle,8>&     particles()     const { return m_particles;     } //!< Get list of particles
    const DataList<GenVertex,8>&       vertices()      const { return m_vertices;      } //!< Get list of vertices

    const GenEventData&                data()          const { return m_data; } //!< Get event data
//
// Fields
//
private:
    int                     m_print_precision; //!< Printout precision

    DataList<GenParticle,8> m_particles;       //!< List of particles
    DataList<GenVertex,8>   m_vertices;        //!< List of vertices

    GenEventData            m_data;            //!< Event data
};

} // namespace HepMC3

#endif
