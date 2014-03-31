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

    /** Get production vertex
     *  Returns the pointer to production vertex (if exists)
     *  @note Particle can have an ancestor without having a production vertex!
     */
    GenVertex* production_vertex() const;

    /** Create production vertex
     *  This vertex will be automatically added to the event.
     *  If particle already has an ancestor, it will be added to this vertex
     *  incoming particle list
     */
    GenVertex* create_production_vertex();

    /** Get end vertex
     *  Returns the pointer to end vertex (if exists)
     *  @note Particle can have a descendant without having end vertex!
     */
    GenVertex* end_vertex() const;

    /** Create end vertex
     *  This vertex will be automatically added to the event.
     *  If particle already has a descendant, it will be added to this vertex
     *  outgoing particle list
     */
    GenVertex* create_end_vertex();

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

    int    ancestor()                           const { return m_ancestor; }             //!< Get ancestor (vertex or particle) barcode
    void   set_ancestor(int barcode)                  { m_ancestor = barcode; }          //!< Set ancestor (vertex or particle) barcode

    int    descendant()                         const { return m_descendant; }           //!< Get descendant (vertex or particle) barcode
    void   set_descendant(int barcode)                { m_descendant = barcode; }        //!< Set descendant (vertex or particle) barcode

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

    short int version_deleted()                 const { return m_version_deleted; } //!< Get version number when this particle was deleted
    void      set_version_deleted(short int v)        { m_version_deleted = v;    } //!< Set version number when this particle was deleted
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
    int        m_ancestor;              //!< Ancestor (vertex or particle) barcode
    int        m_descendant;            //!< Descendant (vertex or particle) barcode
    double     m_generated_mass;        //!< Generated mass
    bool       m_is_generated_mass_set; //!< Check if generated mass is set
    short int  m_version_deleted;       //!< Version number when this particle was deleted
};

} // namespace HepMC3

#endif
