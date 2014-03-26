#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class HepMC3::GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 *  Contains position in space and list of incoming/outgoing particles.
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenParticle;
class GenEvent;

class GenVertex {
//
// Constructors
//
public:
    /** Default constructor */
    GenVertex();
    /** Default destructor */
    ~GenVertex();

//
// Functions
//
public:
    /** Print information about the vertex
     *  By default prints only vertex-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    /** Add incoming particle
     *  Also adds particle to the parent event
     */
    void add_particle_in (GenParticle *p);

    /** Add outgoing particle
     *  Also adds particle to the parent event
     */
    void add_particle_out(GenParticle *p);

    /** Remove particle from incoming/outgoing list */
    void remove_particle(GenParticle *p);


//
// Accessors
//
public:
    GenEvent* parent_event()                    const { return m_parent_event; }   //!< Get parent event

    /** Set parent event
     *  This will also set the barcode of the vertex.
     *  If this vertex was already in other event, it will be removed from that event
     */
    void set_parent_event(GenEvent *evt);

    /** Get particle barcode
     *  Barcodes are handled solely be events.
     *  Returns zero if particle does not belong to an event
     */
    int barcode()                               const { return m_barcode; }

    vector<GenParticle*>& particles_in()              { return m_particles_in; }   //!< Access incoming particle list
    vector<GenParticle*>& particles_out()             { return m_particles_out; }  //!< Access outgoing particle list
    const vector<GenParticle*>& particles_in()  const { return m_particles_in; }   //!< Get incoming particle list
    const vector<GenParticle*>& particles_out() const { return m_particles_out; }  //!< Get outgoing particle list
//
// Fields
//
private:
    GenEvent *m_parent_event;              //!< Parent event
    int m_barcode;                         //!< Barcode

    vector<GenParticle*> m_particles_in;   //!< Incoming particle list
    vector<GenParticle*> m_particles_out;  //!< Outgoing particle list
};

} // namespace HepMC3

#endif
