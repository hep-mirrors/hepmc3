#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class HepMC3::GenParticle
 *
 *  @class HepMC3::GenParticle
 *  @brief Stores particle-related information
 *
 *  Uses HepMC3::GenParticleData to store particle information
 *
 */
#include "HepMC3/Data/GenParticleData.h"

#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace HepMC3 {

class GenEvent;
class GenVertex;

class GenParticle {

friend class GenEvent;
friend class GenVertex;

//
// Constructors
//
public:
    /** Default constructor */
    GenParticle( const FourVector &momentum = FourVector::ZERO_VECTOR(), int pdg_id = 0, int status = 0 );

//
// Functions
//
public:
    /** Print information about the particle
     *  By default prints only particle-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    /** Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_data->event); }

    /** Cast operator used to check if this is a null particle */
    operator bool() const { return (bool)m_data; }
//
// Accessors
//
public:
    int   barcode()               const { return m_data->index+1;        } //!< Get barcode
    int   pdg_id()                const { return m_data->pdg_id;         } //!< Get PDG ID
    int   status()                const { return m_data->status;         } //!< Get status code
    int   status_subcode()        const { return m_data->status_subcode; } //!< Get status subcode
    const FourVector& momentum()  const { return m_data->momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data->is_mass_set;    } //!< Check if generated mass is set

    /** Get generated mass
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    GenVertex production_vertex() const;           //!< Get production vertex
    GenVertex end_vertex() const;                  //!< Get end vertex

    void set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void set_status(int status);                   //!< Set status code
    void set_status_subcode(int subcode);          //!< Set status subcode
    void set_momentum(const FourVector& momentum); //!< Set momentum
    void set_generated_mass(double m);             //!< Set generated mass
    void unset_generated_mass();                   //!< Declare that generated mass is not set
//
// Fields
//
private:
    shared_ptr<GenParticleData> m_data; //!< Particle data
};

} // namespace HepMC3

#endif
