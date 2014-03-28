#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class HepMC3::GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 *  Contains list of incoming/outgoing particles
 *  and optionally, position in timespace
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
    /** Default destructor
     *
     *  @warning Deleting vertex without first removing it from the event
     *           is improper and may cause segmentation fault!
     */
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
    void delete_particle(GenParticle *p);

//
// Accessors
//
public:
    GenEvent* parent_event()                           { return m_parent_event; }          //!< Get parent event
    void      set_parent_event(GenEvent *parent_event) { m_parent_event = parent_event; }  //!< Set parent event

    /** Get vertex barcode
     *  Barcodes are handled solely be events.
     *  Returns zero if particle does not belong to an event
     */
    int barcode()                               const { return m_barcode; }

    /** Set particle barcode
     *  This can be done only once, when particle is added to the event.
     *  Returns false if barcode is already set.
     */
    bool   set_barcode(int barcode);

    vector<GenParticle*>& particles_in()              { return m_particles_in; }   //!< Access incoming particle list
    vector<GenParticle*>& particles_out()             { return m_particles_out; }  //!< Access outgoing particle list
    const vector<GenParticle*>& particles_in()  const { return m_particles_in; }   //!< Get incoming particle list
    const vector<GenParticle*>& particles_out() const { return m_particles_out; }  //!< Get outgoing particle list
//
// Fields
//
public:
    /** GenVertex topological compare
     *  Used for topological sort of GenVertex list.
     *  Compares highest barcode of incoming particles
     */
    struct topological_compare {
        /** Check if v1 < v2 in topological sense */
        bool operator() (HepMC3::GenVertex *v1, HepMC3::GenVertex *v2);
    };
private:
    GenEvent *m_parent_event;              //!< Parent event
    int m_barcode;                         //!< Barcode

    vector<GenParticle*> m_particles_in;   //!< Incoming particle list
    vector<GenParticle*> m_particles_out;  //!< Outgoing particle list
};

} // namespace HepMC3

#endif
