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

namespace HepMC3 {

class GenEvent;
class Filter;

class GenParticle {

friend class GenEventVersion;
friend class GenVertex;
friend class GenEvent;
friend class Filter;

//
// Constructors
//
public:
    /** Default constructor */
    GenParticle(GenEvent *event = NULL);
//
// Functions
//
public:
    /** Print information about the particle
     *  By default prints only particle-related information
     *  event_listing_format = true is used by event for formatted output
     */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

//
// Accessors
//
public:
    int    production_vertex()                  const { return m_production_vertex; }    //!< Get production vertex
    int    end_vertex()                         const { return m_end_vertex; }           //!< Get end vertex
    int    barcode()                            const { return m_barcode; }              //!< Get barcode

    int    pdg_id()                             const { return m_pdgid; }                //!< Get PDG ID
    void   set_pdg_id(int id)                         { m_pdgid = id; }                  //!< Set PDG ID

    int    status()                             const { return m_status; }               //!< Get status code
    void   set_status(int status)                     { m_status = status; }             //!< Set status code

    int    status_subcode()                     const { return m_status_subcode; }       //!< Get status subcode
    void   set_status_subcode(int subcode)            { m_status_subcode = subcode; }    //!< Set status subcode

    const  FourVector& momentum()               const { return m_momentum; }             //!< Get momentum
    FourVector&        momentum()                     { return m_momentum; }             //!< Set momentum by reference
    void   set_momentum(const FourVector& momentum); //!< Set momentum

    /** Get generated mass
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    void   set_generated_mass(double m)               { m_generated_mass = m;   m_is_generated_mass_set = true;  } //!< Set generated mass
    void   unset_generated_mass()                     { m_generated_mass = 0.0; m_is_generated_mass_set = false; } //!< Declare that generated mass is not set
    bool   is_generated_mass_set()                    { return m_is_generated_mass_set; }                          //!< Check if genereted mass is set

//
// Fields
//
private:
    GenEvent  *m_event;                 //!< Parent event
    int        m_production_vertex;     //!< Production vertex
    int        m_end_vertex;            //!< End vertex
    FourVector m_momentum;              //!< Momentum
    int        m_pdgid;                 //!< PDG ID
    int        m_status;                //!< Status
    int        m_status_subcode;        //!< Status subcode
    int        m_barcode;               //!< Barcode
    double     m_generated_mass;        //!< Generated mass
    bool       m_is_generated_mass_set; //!< Check if generated mass is set
    unsigned short int  m_version_created;  //!< Version number when this particle was created
    unsigned short int  m_version_deleted;  //!< Version number when this particle was deleted
    int                 m_next_version;     //!< Barcode of the next version of this particle
};

} // namespace HepMC3

#endif
