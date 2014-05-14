#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H
/**
 *  @file GenParticle.h
 *  @brief Definition of \b class GenParticle
 *
 *  @class HepMC3::GenParticle
 *  @brief Stores particle-related information
 *
 */
#include "HepMC3/Data/SmartPointer.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/FourVector.h"
#include <iostream>
#include <vector>

namespace HepMC3 {

class GenEvent;

class GenParticle {

friend class GenEvent;
friend class GenVertex;
friend class SmartPointer<GenParticle>;

//
// Constructors
//
public:
    /** @brief Default constructor */
    GenParticle( const FourVector &momentum = FourVector::ZERO_VECTOR(), int pdg_id = 0, int status = 0 );

//
// Functions
//
public:
    /** @brief Print information about the particle
     *
     *  By default prints only particle-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    /** @brief Check if this vertex belongs to an event */
    bool in_event() const { return (bool)(m_event); }

//
// Accessors
//
public:
    GenEvent* parent_event()      const { return m_event;   } //!< Get parent event
    int       barcode()           const { return m_index+1; } //!< Get barcode
    const GenParticleData& data() const { return m_data;    } //!< Get particle data

    int   pdg_id()                const { return m_data.pdg_id;         } //!< Get PDG ID
    int   status()                const { return m_data.status;         } //!< Get status code
    int   status_subcode()        const { return m_data.status_subcode; } //!< Get status subcode
    const FourVector& momentum()  const { return m_data.momentum;       } //!< Get momentum
    bool  is_generated_mass_set() const { return m_data.is_mass_set;    } //!< Check if generated mass is set

    /** @brief Get generated mass
     *
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    GenVertexPtr production_vertex() const;        //!< Get production vertex
    GenVertexPtr end_vertex() const;               //!< Get end vertex

    void set_pdg_id(int pdg_id);                   //!< Set PDG ID
    void set_status(int status);                   //!< Set status code
    void set_status_subcode(int subcode);          //!< Set status subcode
    void set_momentum(const FourVector& momentum); //!< Set momentum
    void set_generated_mass(double m);             //!< Set generated mass
    void unset_generated_mass();                   //!< Declare that generated mass is not set

protected:
    void set_production_vertex( const shared_ptr<GenVertex> &v ); //!< Set production vertex
    void set_end_vertex       ( const shared_ptr<GenVertex> &v ); //!< Set end vertex
//
// Fields
//
private:
    GenEvent              *m_event; //!< Parent event
    unsigned int           m_index; //!< Index
    GenParticleData        m_data;  //!< Particle data

    weak_ptr<GenVertex>    m_production_vertex; //!< Production vertex
    weak_ptr<GenVertex>    m_end_vertex;        //!< End vertex
    weak_ptr<GenParticle>  m_this;              //!< Pointer to shared pointer managing this particle
};

} // namespace HepMC3

#endif
