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
 */
#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenEvent;
class GenParticle;
class GenEventVersion;

class GenVertex {

friend class GenEventVersion;
friend class GenEvent;

//
// Constructors
//
public:
    /** Default constructor */
    GenVertex(GenEvent *event = NULL);

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
    void add_particle_in (GenParticle &p);

    /** Add outgoing particle
     *  Also adds particle to the parent event
     */
    void add_particle_out(GenParticle &p);

//
// Accessors
//
public:
    int barcode()                       const { return m_barcode; }        //!< Get barcode

    const vector<int>&  particles_in()  const { return m_particles_in; }   //!< Get incoming particle list
    const vector<int>&  particles_out() const { return m_particles_out; }  //!< Get outgoing particle list

//
// Fields
//
private:
    GenEvent    *m_event;         //!< Parent event
    int          m_barcode;       //!< Barcode
    vector<int>  m_particles_in;  //!< Incoming particle list
    vector<int>  m_particles_out; //!< Outgoing particle list
    unsigned short int m_version_created;  //!< Version number when this vertex was created
    unsigned short int m_version_deleted;  //!< Version number when this vertex was deleted
    int                m_next_version;     //!< Barcode of the next version of this vertex
};

} // namespace HepMC3

#endif
