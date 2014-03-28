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
    /** Default destructor */
    ~GenEvent();

//
// Functions
//
public:
    /** Print information about the event and list full event tree */
    void print( std::ostream& ostr = std::cout ) const;

    /** Add particle to the event
     *  @note Results in an error if this particle already belongs to an event
     */
    void add_particle(GenParticle *p);

    /** Add vertex to the event
     *  Checks if all incoming/outgoing particles already belong to this
     *  event and adds them if needed.
     *  @note Results in an error if either this vertex or any particle that
     *        needs to be added belongs to other event
     */
    void add_vertex(GenVertex *v);

    /** Remove particle from the event
     *  This will not delete the particle but will remove it from
     *  the event and set its barcode to 0
     */
    void delete_particle(GenParticle *p);

    /** Remove vertex from the event
     *  This will not delete the vertex but will remove it from
     *  the event and set its barcode to 0
     */
    void delete_vertex(GenVertex *v);

    /** Create new version
     *  Sets new version as current version
     */
    void create_new_version(const char *name);
//
// Accessors
//
public:
    int  event_number()                     const { return m_event_number; } //!< Get event number
    void set_event_number(int no)                 { m_event_number = no; }   //!< Set event number

    int get_last_particle_barcode()         const { return m_last_particle; } //!< Get barcode of last particle
    int get_last_vertex_barcode()           const { return m_last_vertex;   } //!< Get barcode of last vertex

    const vector<GenParticle*>& particles() const { return m_versions[m_current_version].particles(); } //!< Access particle list
    const vector<GenVertex*>&   vertices()  const { return m_versions[m_current_version].vertices();  } //!< Access vertex list

    int  current_version()                  const { return m_current_version; } //!< Get current version
    void set_current_version(int ver);                                          //!< Set current version

    int  last_version()                     const { return m_last_version; }    //!< Get last version
//
// Fields
//
private:
    int m_event_number;       //!< Event number
    int m_last_particle;      //!< Barcode of the last particle in the event
    int m_last_vertex;        //!< Barcode of the last vertex in the event
    int m_current_version;    //!< Index of current version used
    int m_last_version;       //!< Index of the last version of the event

    vector<GenEventVersion> m_versions; //!< version list
};

} // namespace HepMC3

#endif
