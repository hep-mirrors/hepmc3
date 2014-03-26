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

    /** Remove particle from the event
     *  This will not delete the particle but will remove it from
     *  the event and set its barcode to 0
     */
    void remove_particle(GenParticle *p);

    /** Remove vertex from the event
     *  This will not delete the vertex but will remove it from
     *  the event and set its barcode to 0
     */
    void remove_vertex(GenVertex *v);
private:
    /** Find index of the particle in the container
     *  Returns -1 if particle does not exist in the container
     */
    int find_particle_index(GenParticle *p) const;

    /** Find index of the vertex in the container
     *  Returns -1 if vertex does not exist in the container
     */
    int find_vertex_index(GenVertex *p) const;
//
// Accessors
//
public:
    int event_number()                      const { return m_event_number; } //!< Get event number
    void set_event_number(int no)                 { m_event_number = no; }   //!< Set event number

    int get_highest_particle_barcode()      const { return m_highest_particle; } //!< Get highest particle barcode
    int get_lowest_vertex_barcode()         const { return m_lowest_vertex;    } //!< Get lowest vertex barcode

    vector<GenParticle*>& particles()             { return m_particles; }    //!< Access particle list
    vector<GenVertex*>&   vertices()              { return m_vertices; }     //!< Access vertex list

    const vector<GenParticle*>& particles() const { return m_particles; }    //!< Access particle list
    const vector<GenVertex*>&   vertices()  const { return m_vertices; }     //!< Access vertex list
//
// Fields
//
private:
    int m_event_number;     //!< Event number
    int m_highest_particle; //!< Barcode of the last particle in the event
    int m_lowest_vertex;    //!< Barcode of the last vertex in the event

    vector<GenParticle*> m_particles; //!< Particle list
    vector<GenVertex*>   m_vertices;  //!< Vertex list
};

} // namespace HepMC3

#endif
