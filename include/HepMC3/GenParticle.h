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

class GenParticle {
//
// Constructors
//
public:
    /** Default constructor */
    GenParticle();
    /** Constructs particle from basic information */
    GenParticle(FourVector momentum, int pdgid, int status);
    /** Default destructor */
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
    /** Get particle barcode
     *  Returns zero if particle does not belong to an event
     */
    int    barcode()                            const { return m_barcode; }
    void   set_barcode(int barcode)                   { m_barcode = barcode; }           //!< Set barcode @todo Remove this function!!

    int    pdg_id()                             const { return m_pdgid; }                //!< Get PDG ID
    void   set_pdg_id(int id)                         { m_pdgid = id; }                  //!< Set PDG ID

    int    status()                             const { return m_status; }               //!< Get status code
    void   set_status(int status)                     { m_status = status; }             //!< Set status code

    int    production_vertex_barcode()          const { return m_production_vertex; }    //!< Get production vertex barcode
    void   set_production_vertex_barcode(int barcode) { m_production_vertex = barcode; } //!< Set production vertex barcode @todo Replace with proper ancestor setter

    int    end_vertex_barcode()                 const { return m_end_vertex; }           //!< Get end vertex barcode
    void   set_end_vertex_barcode(int barcode)        { m_end_vertex = barcode; }        //!< Set end vertex barcode @todo Replace with proper child setter

    const  FourVector& momentum()               const { return m_momentum; }             //!< Get momentum
    void   set_momentum(FourVector& momentum)         { m_momentum  = momentum; }        //!< Set momentum

    double generated_mass()                     const { return m_generated_mass; }       //!< Get generated mass
    void   set_generated_mass(double m)               { m_generated_mass = m; }          //!< Set generated mass

//
// Fields
//
private:
    FourVector m_momentum;    //!< Momentum
    int m_pdgid;              //!< PDG ID
    int m_status;             //!< Status @todo Add second status code
    int m_barcode;            //!< Barcode
    int m_production_vertex;  //!< Production vertex barcode
    int m_end_vertex;         //!< End vertex barcode
    double m_generated_mass;  //!< Generated mass @todo Add check if this value is set
};

} // namespace HepMC3

#endif
