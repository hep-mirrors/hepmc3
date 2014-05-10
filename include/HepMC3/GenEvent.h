#ifndef  HEPMC3_GENEVENT_H
#define  HEPMC3_GENEVENT_H
/**
 *  @file GenEvent.h
 *  @brief Definition of \b class GenEvent
 *
 *  @class HepMC3::GenEvent
 *  @brief Stores event-related information
 *
 *  Manages GenParticle and GenVertex objects
 *
 */
#include "HepMC3/Data/SmartPointer.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenEventData;

class GenEvent {
//
// Constructors
//
public:
    /** @brief Default constructor */
    GenEvent();

//
// Functions
//
public:
    /** @brief Print current version of the event */
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

    /** @brief Delete particle */
    void delete_particle( const GenParticlePtr &p );

    /** @brief Delete vertex */
    void delete_vertex( const GenVertexPtr &v );

    /** @brief Reserve memory for particles and vertices
     *
     *  Helps optimize event creation when size of the event is known beforehand
     */
    void reserve(unsigned int particles, unsigned int vertices = 0);

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

    const vector<GenParticlePtr>& particles()  const { return m_particles;            } //!< Get list of particles
    const vector<GenVertexPtr>&   vertices()   const { return m_vertices;             } //!< Get list of vertices

    GenEventData* serializable_data() const; //!< Create event data that can be used for serialization @todo This is temporary function for benchmarking. Change it to proper class

//
// Deprecated functions
//
public:
    __attribute__((deprecated("Use GenParticlePtr instead of GenParticle*"))) void add_particle( GenParticle *p ); //!< Add particle by raw pointer @deprecated Use GenEvent::add_particle( const GenParticlePtr& ) instead
    __attribute__((deprecated("Use GenVertexPtr instead of GenVertex*")))     void add_vertex  ( GenVertex *v );   //!< Add vertex by raw pointer   @deprecated Use GenEvent::add_vertex( const GenVertexPtr& ) instead

//
// Fields
//
private:
    int                         m_event_number;    //!< Event number
    int                         m_print_precision; //!< Printout precision
    std::vector<GenParticlePtr> m_particles;       //!< List of particles
    std::vector<GenVertexPtr>   m_vertices;        //!< List of vertices
};

} // namespace HepMC3

#endif
