#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class HepMC3::GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 *  Uses HepMC3::GenVertexData to store vertex information
 *
 */
#include "HepMC3/Data/GenVertexData.h"

#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using std::vector;

namespace HepMC3 {

class GenEvent;
class GenParticle;

class GenVertex {

friend class GenEvent;
friend class GenParticle;

//
// Constructors
//
public:
    /** Default constructor */
    GenVertex( const FourVector& position = FourVector::ZERO_VECTOR() );

protected:
    /** Constructor based on GenVertexData
     *  Used by GenParticle to obtain production/end vertex, which can be null
     */
    GenVertex( const shared_ptr<GenVertexData> &data );

//
// Functions
//
public:
    /** Print information about the vertex
     *  By default prints only vertex-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    /** Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_data->event); }

    /** Cast operator used to check if this is a null vertex */
    operator bool() const { return (bool)m_data; }

//
// Accessors
//
public:
    int barcode() const { return -(((int)m_data->index)+1);  } //!< Get barcode

    /** Return barcode used in serialization
     *  If the vertex has at most one incoming particle, it might not be serialized
     *  if it does not contain any m_data. In such cases, its barcode used for
     *  serialization can be 0 or can be the barcode of its sole incoming particle
     */
    int serialization_barcode() const;

    void add_particle_in (GenParticle &p); //!< Add incoming particle
    void add_particle_out(GenParticle &p); //!< Add outgoing particle

    const vector<GenParticle>& particles_in()  const { return m_data->particles_in;  } //!< Get list of incoming particles
    const vector<GenParticle>& particles_out() const { return m_data->particles_out; } //!< Get list of outgoing particles

    /** Get position
     *  Returns position of this vertex. If position is not set, searches
     *  production vertices of ancestors to find position.
     *  Returns HepMC3::FourVector(0,0,0,0) if no position information found.
     */
    const FourVector& position() const;
    void              set_position(const FourVector& new_pos); //!< Set position
//
// Fields
//
private:
    shared_ptr<GenVertexData> m_data; //!< Vertex data
};

} // namespace HepMC3

#endif
