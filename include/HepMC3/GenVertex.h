#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H
/**
 *  @file GenVertex.h
 *  @brief Definition of \b class HepMC3::GenVertex
 *
 *  @class HepMC3::GenVertex
 *  @brief Stores vertex-related information
 *
 *  Contains lists of incoming/outgoing particles
 *
 */
#include "HepMC3/Data/GenVertexData.h"

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenEvent;
class GenParticle;

class GenVertex {

friend class GenEvent;

//
// Constructors
//
protected:
    /** Default constructor */
    GenVertex(GenEvent &event, int data_index, GenVertexData &data);

//
// Functions
//
public:
    /** Print information about the vertex
     *  By default prints only vertex-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false, int version = 255) const;

    bool is_deleted() const { return ( m_version_deleted != 255 ); } //!< Check if this vertex is deleted
    void mark_deleted();                                             //!< Mark this vertex as deleted
//
// Accessors
//
public:
    int   barcode()               const { return -(m_data_index+1);  } //!< Get barcode

    /** Return barcode used in serialization
     *  If the vertex has at most one incoming particle, it might not be serialized
     *  if it does not contain any data. In such cases, its barcode used for
     *  serialization can be 0 or can be the barcode of its sole incoming particle
     */
    int serialization_barcode() const;

    unsigned short int version_created() const { return m_version_created;      } //!< Version in which this particle was created
    unsigned short int version_deleted() const { return m_version_deleted;      } //!< Version in which this particle was deleted
    bool               has_new_version() const { return m_last_version != this; } //!< Check if this is the last version of this vertex

    void add_particle_in (GenParticle &p); //!< Add incoming particle
    void add_particle_out(GenParticle &p); //!< Add outgoing particle

    const vector<GenParticle*> particles_in()  const { return m_particles_in;  } //!< Get list of incoming particles
    const vector<GenParticle*> particles_out() const { return m_particles_out; } //!< Get list of outgoing particles
    const GenVertexData& data()                const { return m_data;          } //!< Get vertex data

//
// Fields
//
private:
    GenEvent             &m_event;           //!< Parent event
    unsigned short int    m_version_created; //!< Version created
    unsigned short int    m_version_deleted; //!< Version deleted
    unsigned int          m_data_index;      //!< Index in particle data container
    GenVertexData        &m_data;            //!< Particle data
    GenVertex            *m_last_version;    //!< Pointer to the last version of this vertex
    vector<GenParticle*>  m_particles_in;    //!< Incoming particle list
    vector<GenParticle*>  m_particles_out;   //!< Outgoing particle list
    bool                  m_is_required;     //!< Does this vertex needs to be serialized
};

} // namespace HepMC3

#endif
