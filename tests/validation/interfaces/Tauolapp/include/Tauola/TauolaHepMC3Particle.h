#ifndef _TauolaHepMC3Particle_h_included_
#define _TauolaHepMC3Particle_h_included_

#include <vector>

#include "Tauola/TauolaParticle.h"
#include "HepMC3/GenParticle.h"

namespace Tauolapp
{

class TauolaHepMC3Event;

class TauolaHepMC3Particle: public TauolaParticle {

//
// Constructors
//
public:
    /** Constructor which keeps a pointer to the HepMC3::GenParticle*/
    TauolaHepMC3Particle( const HepMC3::GenParticle &particle);

    ~TauolaHepMC3Particle();

//
// Functions
//
public:
    /** Returns the HepMC3::GenParticle */
    HepMC3::GenParticle& getHepMC3() { return m_particle; }

    /** Remove the decay branch from the event record and reset the particle status code to stable. */
    void undecay();

    /** Set the mothers of this particle via a vector of TauolaParticle*/
    void setMothers(std::vector<TauolaParticle*> mothers);

    /** Set the daughters of this particle via a vector of TauolaParticle*/
    void setDaughters(std::vector<TauolaParticle*> daughters);

    /** Returns the mothers of this particle via a vector of TauolaParticle */
    std::vector<TauolaParticle*> getMothers();

    /** Returns the daughters of this particle via a vector of TauolaParticle */
    std::vector<TauolaParticle*> getDaughters();

    /** Check that the 4 momentum in conserved at the vertices producing
      and ending this particle */
    void checkMomentumConservation();

    /** Overriding of TauolaParticle decayEndgame method.
      Converts the momentum and length units
      and sets the vector (X,T) position */
    void decayEndgame();

    /** Create a new particle of type TauolaHepMC3Particle, with the given
      properties. The new particle bares no relations to this
      particle, but it provides a way of creating a instance of
      this derived class. eg. createNewParticle() is used inside
      filhep_() so that a TauolaHepMC3Particle can be created without
      the method having explicit knowledge of the TauolaHepMC3Particle
      class */
    TauolaHepMC3Particle * createNewParticle(int pdg_id, int status, double mass,
                                             double px, double py,
                                             double pz, double e);

//
// Accessors
//
public:
    /** Get parent event */
    TauolaHepMC3Event* parent_event()             { return m_parent_event; }

    /** Set parent event */
    void set_parent_event(TauolaHepMC3Event *evt) { m_parent_event = evt; }

    double getPx()                { if(!m_particle) return 0; return m_particle.momentum().px(); }   //!< Get px
    double getPy()                { if(!m_particle) return 0; return m_particle.momentum().py(); }   //!< Get py
    double getPz()                { if(!m_particle) return 0; return m_particle.momentum().pz(); }   //!< Get pz
    double getE()                 { if(!m_particle) return 0; return m_particle.momentum().e(); }    //!< Get energy

    void   setPx( double px ); //!< Set px
    void   setPy( double py ); //!< Set py
    void   setPz( double pz ); //!< Set pz
    void   setE( double e );   //!< Set energy

    int    getPdgID()             { if(!m_particle) return 0; return m_particle.pdg_id(); }          //!< Get PDG ID
    void   setPdgID(int pdg_id)   { if(!m_particle) return;   m_particle.set_pdg_id(pdg_id); }       //!< Set PDG ID

    int    getStatus()            { if(!m_particle) return 0; return m_particle.status(); }          //!< Get status
    void   setStatus(int status)  { if(!m_particle) return;   m_particle.set_status(status); }       //!< Set status

    double getMass()              { if(!m_particle) return 0; return m_particle.generated_mass(); }  //!< Get mass
    void   setMass(double mass)   { if(!m_particle) return;   m_particle.set_generated_mass(mass); } //!< Set mass

    int    getBarcode()           { if(!m_particle) return 0; return m_particle.barcode(); }         //!< Get barcode

    /** Print some information about this particle to standard output */
    void   print()                { if(!m_particle) return; m_particle.print(); }

//
// Fields
//
private:
    TauolaHepMC3Event            *m_parent_event; //!< Parent event
    HepMC3::GenParticle           m_particle;     //!< HepMC3::GenParticle particle
    std::vector<TauolaParticle*>  m_mothers;      //!< List of mothers
    std::vector<TauolaParticle*>  m_daughters;    //!< List of daughters

    /** List to keep track of new particles which have been
      created from this one, so we can call their destructor later */
    std::vector<TauolaParticle*> m_created_particles;
};

} // namespace Tauolapp
#endif

