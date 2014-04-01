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
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include "HepMC3/FourVector.h"

namespace HepMC3 {

class GenVertex;
class GenEvent;

class GenParticle {

friend class GenVertex;
friend class GenEvent;

//
// Constructors
//
public:
    /** Default constructor */
    GenParticle();
    /** Constructs particle from basic information */
    GenParticle(FourVector momentum, int pdgid, int status);
    /** Default destructor
     *
     *  @warning Deleting particle without first removing it from the event
     *           is improper and may cause segmentation fault!
     */
    ~GenParticle();

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
    GenVertex* production_vertex()               const { return m_production_vertex; }   //!< Get production vertex
    GenVertex* end_vertex()                      const { return m_end_vertex; }          //!< Get end vertex

    int    barcode()                            const { return m_barcode; }              //!< Get barcode

    int    pdg_id()                             const { return m_pdgid; }                //!< Get PDG ID
    void   set_pdg_id(int id)                         { m_pdgid = id; }                  //!< Set PDG ID

    int    status()                             const { return m_status; }               //!< Get status code
    void   set_status(int status)                     { m_status = status; }             //!< Set status code

    int    status_subcode()                     const { return m_status_subcode; }       //!< Get status subcode
    void   set_status_subcode(int subcode)            { m_status_subcode = subcode; }    //!< Set status subcode

    const  FourVector& momentum()               const { return m_momentum; }             //!< Get momentum
    FourVector&        momentum()                     { return m_momentum; }             //!< Set momentum by reference
    void   set_momentum(FourVector& momentum)         { m_momentum  = momentum; }        //!< Set momentum

    /** Get generated mass
     *  This function will return mass as set by a generator/tool.
     *  If not set, it will return momentum().m()
     */
    double generated_mass() const;

    void   set_generated_mass(double m)               { m_generated_mass = m;   m_is_generated_mass_set = true;  } //!< Set generated mass
    void   unset_generated_mass()                     { m_generated_mass = 0.0; m_is_generated_mass_set = false; } //!< Declare that generated mass is not set
    bool   is_generated_mass_set()                    { return m_is_generated_mass_set; }                          //!< Check if genereted mass is set

protected:
    void      set_production_vertex(GenVertex *v)     { m_production_vertex = v;    }    //!< Set production vertex
    void      set_end_vertex(GenVertex *v)            { m_end_vertex = v;    }           //!< Set end vertex

    short int version_deleted()                 const { return m_version_deleted; }      //!< Get deletion version number
    void      set_version_deleted(short int v)        { m_version_deleted = v;    }      //!< Set deletion version number

    void      set_barcode(int barcode)                { m_barcode = barcode; }           //!< Set barcode
//
// Fields
//
private:
    GenVertex *m_production_vertex;     //!< Production vertex
    GenVertex *m_end_vertex;            //!< End vertex
    FourVector m_momentum;              //!< Momentum
    int        m_pdgid;                 //!< PDG ID
    int        m_status;                //!< Status
    int        m_status_subcode;        //!< Status subcode
    int        m_barcode;               //!< Barcode
    double     m_generated_mass;        //!< Generated mass
    bool       m_is_generated_mass_set; //!< Check if generated mass is set
    short int  m_version_deleted;       //!< Version number when this particle was deleted
};

} // namespace HepMC3

#endif
