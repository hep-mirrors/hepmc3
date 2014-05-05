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
    void print( std::ostream& ostr = std::cout ) const;

    /** Dump the whole content of the event memory. Useful for debugging */
    void dump() const;

    /** Add particle */
    void add_particle(GenParticle &p);

    /** Add vertex */
    void add_vertex(GenVertex &v);

    /** Add whole tree in topological order
     *  This function will find the beam particles (particles
     *  that have no production vertices or their production vertices
     *  have no particles) and will add the whole decay tree starting from
     *  these particles.
     *
     *  @note Any particles on this list that do not belong to the tree
     *        will be ignored.
     */
    void add_tree( const vector<GenParticle> &particles );

    /** Delete particle */
    void delete_particle(GenParticle &p);

    /** Delete vertex */
    void delete_vertex(GenVertex &v);

    /** Reserve memory for particles and vertices
     *  Helps optimize event creation when size of the event is known beforehand
     */
    void reserve(unsigned int particles, unsigned int vertices = 0);
//
// Accessors
//
public:
    int  print_precision()                  const { return m_print_precision;      } //!< Get printout precision. Default is 2
    void set_print_precision(int precision)       { m_print_precision = precision; } //!< Set printout precision. Default is 2

    int  event_number()                     const { return m_event_number;         } //!< Get event number
    void set_event_number(int no)                 { m_event_number = no;           } //!< Set event number

    unsigned int particles_count()          const { return m_particles.size();     } //!< Get number of particles
    unsigned int vertices_count()           const { return m_vertices.size();      } //!< Get number of vertices

    const vector<GenParticle>& particles()  const { return m_particles;            } //!< Get list of particles
    const vector<GenVertex>&   vertices()   const { return m_vertices;             } //!< Get list of vertices
    //GenEventData* get_event();
//
// Fields
//
private:
    int                       m_event_number;    //!< Event number
    int                       m_print_precision; //!< Printout precision
    std::vector<GenParticle>  m_particles;       //!< List of particles
    std::vector<GenVertex>    m_vertices;        //!< List of vertices
};

} // namespace HepMC3

#endif
