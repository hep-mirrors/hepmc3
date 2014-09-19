#ifndef  HEPMC_GENVERTEX_H
#define  HEPMC_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class GenVertex
 *
 *  @class HepMC::GenVertex
 *  @brief Stores vertex-related information
 *
 */
#include "HepMC/Data/SmartPointer.h"
#include "HepMC/Data/GenVertexData.h"
#include "HepMC/FourVector.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC {

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

    /** @brief Constructor based on vertex data */
    GenVertex( const GenVertexData &data );

//
// Functions
//
public:
    /** @brief Print information about the vertex */
    void print( std::ostream& ostr = std::cout ) const;

    /** @brief Print vertex content for selected version */
    void print_version( unsigned char version, std::ostream& ostr = std::cout ) const;

    /** @brief Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_event); }

    /** @brief Create new version of this particle
     *
     *  @return Pointer to the new version of this vertex or null pointer if:
     *  - this vertex does not belong to an event
     *  - this vertex already belongs to the last version
     *  - newer version of this vertex already exists
     *  - vertex has been marked as deleted
     */
    GenVertexPtr new_version();

    /** @brief Mark vertex as deleted */
    void mark_deleted();

//
// Accessors
//
public:
    GenEvent*            parent_event() const { return m_event; } //!< Get parent event
    int                  id()           const { return m_id;    } //!< Get vertex id
    const GenVertexData& data()         const { return m_data;  } //!< Get vertex data

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

    /** @brief Get barcode
     *
     *  Currently barcode = id
     *  @todo Write proper barcode once we decide how it should look like
     */
    int barcode() const { return m_id; }

    unsigned char version_created() const { return m_version_created; } //!< Get version created
    unsigned char version_deleted() const { return m_version_deleted; } //!< Get version deleted

    /** @brief Check if this vertex exists in selected version */
    bool is_in_version(unsigned char version) const { return   m_version_created <= version &&
                                                             (!m_version_deleted || m_version_deleted>version); }
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
    GenEvent      *m_event; //!< Parent event
    int            m_id;    //!< Vertex id
    GenVertexData  m_data;  //!< Vertex data

    unsigned char m_version_created; //!< Version created
    unsigned char m_version_deleted; //!< Version deleted
    GenVertexPtr  m_next_version;    //!< Next version of this vertex

    vector<GenParticlePtr>  m_particles_in;  //!< Incoming particle list
    vector<GenParticlePtr>  m_particles_out; //!< Outgoing particle list
    weak_ptr<GenVertex>     m_this;          //!< Pointer to shared pointer managing this vertex
};

} // namespace HepMC

#endif
