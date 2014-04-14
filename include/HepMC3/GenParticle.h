#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class HepMC3::GenParticle
 *
 *  @class HepMC3::GenParticle
 *  @brief Stores particle-related information
 *
 *  Uses HepMC3::FourVector class to store momentum
 *
 */
#include "HepMC3/FourVector.h"
#include <iostream>
#include <vector>

namespace HepMC3 {

class GenVertex;
class GenEvent;
class Filter;

/**
 *  @struct HepMC3::GenParticleData
 *  @brief Stores serializable particle information
 */
struct GenParticleData {
    int          pdg_id;            //!< PDG ID
    int          ancestor;          //!< Ancestor of this particle
    FourVector   momentum;          //!< Momentum
    int          status;            //!< Status
    int          status_subcode;    //!< Status subcode
    double       mass;              //!< Generated mass (if set)
    bool         is_mass_set;       //!< Check if generated mass is set

    /** Print particle data content */
    void print() const;
};

class GenParticle {

friend class GenVertex;
friend class GenEvent;
friend class Filter;

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
    const FourVector& momentum()  const { return m_data.momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data.is_mass_set;    } //!< Check if generated mass is set
    const GenParticleData& data() const { return m_data;                } //!< Get particle data

    /** Get generated mass
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    const GenVertex* production_vertex() const { return m_production_vertex; } //!< Get production vertex
    const GenVertex* end_vertex()        const { return m_end_vertex;        } //!< Get end vertex

    void  set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void  set_status(int status);                   //!< Set status code
    void  set_status_subcode(int subcode);          //!< Set status subcode
    void  set_momentum(const FourVector& momentum); //!< Set momentum
    void  set_generated_mass(double m);             //!< Set generated mass
    void  unset_generated_mass();                   //!< Declare that generated mass is not set

//
// Fields
//
private:
    GenEvent           &m_event;                   //!< Parent event
    unsigned short int  m_version_created;         //!< Version created
    unsigned short int  m_version_deleted;         //!< Version deleted
    GenVertex          *m_production_vertex;       //!< Production vertex
    GenVertex          *m_end_vertex;              //!< End vertex
    unsigned int        m_data_index;              //!< Index in particle data container
    GenParticleData    &m_data;                    //!< Particle data
    GenParticle        *m_last_version;            //!< Pointer to the last version of this particle
};

} // namespace HepMC3

#endif
