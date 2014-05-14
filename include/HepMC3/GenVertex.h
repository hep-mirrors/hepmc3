#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 */
#include "HepMC3/Data/SmartPointer.h"
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/FourVector.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenEvent;

class GenVertex {

friend class GenEvent;
friend class SmartPointer<GenVertex>;

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenVertex( const FourVector& position = FourVector::ZERO_VECTOR() );

//
// Functions
//
public:
    /** @brief Print information about the vertex
     *
     *  By default prints only vertex-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    /** @brief Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_event); }

//
// Accessors
//
public:
    GenEvent* parent_event()    const { return m_event;              } //!< Get parent event
    int       barcode()         const { return -(((int)m_index)+1);  } //!< Get barcode
    const GenVertexData& data() const { return m_data;               } //!< Get vertex data

    /** @brief Return barcode used in serialization
     *
     *  If the vertex has at most one incoming particle, it might not be serialized
     *  if it does not contain any m_data. In such cases, its barcode used for
     *  serialization can be 0 or can be the barcode of its sole incoming particle
     */
    int serialization_barcode() const;

    void add_particle_in ( const GenParticlePtr &p ); //!< Add incoming particle
    void add_particle_out( const GenParticlePtr &p ); //!< Add outgoing particle

    const vector<GenParticlePtr>& particles_in()  const { return m_particles_in;  } //!< Get list of incoming particles
    const vector<GenParticlePtr>& particles_out() const { return m_particles_out; } //!< Get list of outgoing particles

    /** @brief Get position
     *
     *  Returns position of this vertex. If position is not set, searches
     *  production vertices of ancestors to find position.
     *  Returns FourVector(0,0,0,0) if no position information found.
     */
    const FourVector& position() const;
    void              set_position(const FourVector& new_pos); //!< Set position

//
// Deprecated functions
//
public:
    __attribute__((deprecated("Use GenParticlePtr instead of GenParticle*"))) void add_particle_in ( GenParticle *p ) { add_particle_in (GenParticlePtr(p)); } //!< Add incoming particle by raw pointer @deprecated Use GenVertex::add_particle_in( const GenParticlePtr &p ) instead
    __attribute__((deprecated("Use GenParticlePtr instead of GenParticle*"))) void add_particle_out( GenParticle *p ) { add_particle_out(GenParticlePtr(p)); } //!< Add outgoing particle by raw pointer @deprecated Use GenVertex::add_particle_out( const GenParticlePtr &p ) instead

//
// Fields
//
private:
    GenEvent               *m_event;         //!< Parent event
    unsigned int            m_index;         //!< Index
    GenVertexData           m_data;          //!< Vertex data
    vector<GenParticlePtr>  m_particles_in;  //!< Incoming particle list
    vector<GenParticlePtr>  m_particles_out; //!< Outgoing particle list
    weak_ptr<GenVertex>     m_this;          //!< Pointer to shared pointer managing this vertex
};

} // namespace HepMC3

#endif
