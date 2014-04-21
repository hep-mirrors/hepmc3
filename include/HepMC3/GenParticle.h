#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class HepMC3::GenParticle
 *
 *  @class HepMC3::GenParticle
 *  @brief Stores particle-related information
 *
 *  Uses HepMC3::GenParticleData to store serializable particle information
 *
 */
#include "HepMC3/Data/GenParticleData.h"

#include <iostream>
#include <vector>
#include <limits>

namespace HepMC3 {

class GenVertex;
class GenEvent;

class GenParticle {

friend class GenEvent;

//
// Constructors
//
protected:
    /** Default constructor */
    GenParticle(GenEvent &event, int data_index, GenParticleData &data);
//
// Functions
//
public:
    /** Print information about the particle
     *  By default prints only particle-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    bool is_deleted() const { return ( m_version_deleted != 255 ); } //!< Check if this particle is deleted
    void mark_deleted();                                             //!< Mark this particle as deleted

//
// Accessors
//
public:
    int   barcode()               const { return m_data_index+1;        } //!< Get barcode
    int   pdg_id()                const { return m_data.pdg_id;         } //!< Get PDG ID
    int   status()                const { return m_data.status;         } //!< Get status code
    int   status_subcode()        const { return m_data.status_subcode; } //!< Get status subcode
    FourVector& momentum()        const { return m_data.momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data.is_mass_set;    } //!< Check if generated mass is set
    const GenParticleData& data() const { return m_data;                } //!< Get particle data

    unsigned char version_created() const { return m_version_created;      } //!< Version in which this particle was created
    unsigned char version_deleted() const { return m_version_deleted;      } //!< Version in which this particle was deleted
    GenParticle*  last_version()    const { return m_last_version;         } //!< Get latest version of this particle
    bool          has_new_version() const { return m_last_version != this; } //!< Check if this is the last version of this particle


    /** Get generated mass
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    GenVertex* production_vertex() const;                   //!< Get production vertex
    void       set_production_vertex( const GenVertex *v ); //!< Set production vertex

    GenVertex* end_vertex() const;                          //!< Get end vertex
    void       set_end_vertex( const GenVertex *v );        //!< Set end vertex

    void set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void set_status(int status);                   //!< Set status code
    void set_status_subcode(int subcode);          //!< Set status subcode
    void set_momentum(const FourVector& momentum); //!< Set momentum
    void set_generated_mass(double m);             //!< Set generated mass
    void unset_generated_mass();                   //!< Declare that generated mass is not set
    void set_data(const GenParticleData& data);    //!< Set particle data
//
// Fields
//
private:
    GenEvent           &m_event;                   //!< Parent event
    unsigned char       m_version_created;         //!< Version created
    unsigned char       m_version_deleted;         //!< Version deleted
    unsigned int        m_data_index;              //!< Index in particle data container
    GenParticleData    &m_data;                    //!< Particle data
    GenParticle        *m_last_version;            //!< Pointer to the last version of this particle
};

} // namespace HepMC3

#endif
