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

class GenEvent;

class GenParticle {
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
    GenEvent* parent_event()                           { return m_parent_event; }          //!< Get parent event
    void      set_parent_event(GenEvent *parent_event) { m_parent_event = parent_event; }  //!< Set parent event

    /** Get particle barcode
     *  Barcodes are handled solely be events.
     *  Returns zero if particle does not belong to an event
     */
    int    barcode()                            const { return m_barcode; }

    /** Set particle barcode
     *  This can be done only once, when particle is added to the event.
     *  Returns false if barcode is already set.
     */
    bool   set_barcode(int barcode);

    int    pdg_id()                             const { return m_pdgid; }                //!< Get PDG ID
    void   set_pdg_id(int id)                         { m_pdgid = id; }                  //!< Set PDG ID

    int    status()                             const { return m_status; }               //!< Get status code
    void   set_status(int status)                     { m_status = status; }             //!< Set status code

    int    status_subcode()                     const { return m_status_subcode; }       //!< Get status subcode
    void   set_status_subcode(int subcode)            { m_status_subcode = subcode; }    //!< Set status subcode

    int    production_vertex_barcode()          const { return m_production_vertex; }    //!< Get production vertex barcode
    void   set_production_vertex_barcode(int barcode) { m_production_vertex = barcode; } //!< Set production vertex barcode @todo Replace with proper ancestor setter

    int    end_vertex_barcode()                 const { return m_end_vertex; }           //!< Get end vertex barcode
    void   set_end_vertex_barcode(int barcode)        { m_end_vertex = barcode; }        //!< Set end vertex barcode @todo Replace with proper child setter

    const  FourVector& momentum()               const { return m_momentum; }             //!< Get momentum
    void   set_momentum(FourVector& momentum)         { m_momentum  = momentum; }        //!< Set momentum

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
    GenEvent  *m_parent_event;          //!< Parent event
    FourVector m_momentum;              //!< Momentum
    int        m_pdgid;                 //!< PDG ID
    int        m_status;                //!< Status
    int        m_status_subcode;        //!< Status subcode
    int        m_barcode;               //!< Barcode
    int        m_production_vertex;     //!< Production vertex barcode
    int        m_end_vertex;            //!< End vertex barcode
    double     m_generated_mass;        //!< Generated mass
    bool       m_is_generated_mass_set; //!< Check if generated mass is set
};

} // namespace HepMC3

#endif
