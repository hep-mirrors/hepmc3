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

friend class GenEvent;

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

//
// Accessors
//
public:
    int barcode()                               const { return m_barcode; }        //!< Get barcode

    const vector<GenParticle*>& particles_in()  const { return m_particles_in; }   //!< Get incoming particle list
    const vector<GenParticle*>& particles_out() const { return m_particles_out; }  //!< Get outgoing particle list
protected:
    void set_barcode(int barcode)                     { m_barcode = barcode; }      //!< Set barcode

    short int version_created()                 const { return m_version_created; } //!< Get creation version number
    void      set_version_created(short int v)        { m_version_created = v;    } //!< Set creation version number

    short int version_deleted()                 const { return m_version_deleted; } //!< Get deletion version number
    void      set_version_deleted(short int v)        { m_version_deleted = v;    } //!< Set deletion version number
//
// Fields
//
private:
    int m_barcode;                         //!< Barcode

    vector<GenParticle*> m_particles_in;   //!< Incoming particle list
    vector<GenParticle*> m_particles_out;  //!< Outgoing particle list
    short int            m_version_created;//!< Version number when this vertex was created
    short int            m_version_deleted;//!< Version number when this vertex was deleted
};

} // namespace HepMC3

#endif
