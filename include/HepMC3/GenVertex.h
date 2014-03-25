#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class HepMC3::GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 *  Contains lists of descendants and ancestors.
 *  Does not own particles on these lists
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenParticle;

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
     *  Change ownership of the particle if needed
     */
    void add_particle_in (GenParticle *p);

    /** Add outgoing particle
     *  Change ownership of the particle if needed
     */
    void add_particle_out(GenParticle *p);

//
// Accessors
//
public:
    int barcode()                               const { return m_barcode; }        //!< Get barcode
    void set_barcode(int barcode)                     { m_barcode = barcode; }     //!< Set barcode @todo Remove this function!!

    vector<GenParticle*>& particles_in()              { return m_particles_in; }   //!< Access incoming particle list
    vector<GenParticle*>& particles_out()             { return m_particles_out; }  //!< Access outgoing particle list
    const vector<GenParticle*>& particles_in()  const { return m_particles_in; }   //!< Get incoming particle list
    const vector<GenParticle*>& particles_out() const { return m_particles_out; }  //!< Get outgoing particle list
//
// Fields
//
private:
    int m_barcode;                         //!< Barcode

    vector<GenParticle*> m_particles_in;   //!< Incoming particle list
    vector<GenParticle*> m_particles_out;  //!< Outgoing particle list
};

} // namespace HepMC3

#endif
