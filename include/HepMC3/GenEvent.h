#ifndef  HEPMC3_GENEVENT_H
#define  HEPMC3_GENEVENT_H
/**
 *  @file GenEvent.h
 *  @brief Definition of \b class HepMC3::GenEvent
 *
 *  @class HepMC3::GenEvent
 *  @brief Stores event-related information
 *
 *  Owns all vertices and particles in its lists
 *  They are deleted when the HepMC3::~GenEvent() is invoked
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <vector>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
using std::vector;

namespace HepMC3 {

class GenEvent {
//
// Constructors
//
public:
    /** Default constructor */
    GenEvent();
    /** Default destructor */
    ~GenEvent();

//
// Functions
//
public:
    /** Print information about the event and list full event tree */
    void print( std::ostream& ostr = std::cout ) const;

    /** Add particle to the event
     *  Change ownership of the particle if it already belongs to an event
     */
    void add_particle(GenParticle *p);

    /** Add vertex to the event
     *  Change ownership of the vertex if it already belongs to an event
     */
    void add_vertex(GenVertex *v);

//
// Accessors
//
public:
    int event_number() const                      { return m_event_number; } //!< Get event number
    void set_event_number(int no)                 { m_event_number = no; }   //!< Set event number

    vector<GenParticle*>& particles()             { return m_particles; }    //!< Access particle list
    vector<GenVertex*>&   vertices()              { return m_vertices; }     //!< Access vertex list

    const vector<GenParticle*>& particles() const { return m_particles; }    //!< Access particle list
    const vector<GenVertex*>&   vertices()  const { return m_vertices; }     //!< Access vertex list
//
// Fields
//
private:
    int m_event_number;   //!< Event number
    int m_lowest_barcode; //!< Barcode of the last particle in the event
    int m_lowest_vertex;  //!< Barcode of the last vertex in the event

    vector<GenParticle*> m_particles; //!< Particle list
    vector<GenVertex*>   m_vertices;  //!< Vertex list
};

} // namespace HepMC3

#endif
