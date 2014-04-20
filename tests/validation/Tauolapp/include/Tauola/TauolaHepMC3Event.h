#ifndef _TauolaHepMC3Event_h_included_
#define _TauolaHepMC3Event_h_included_

#include <iostream>
#include "Tauola/TauolaEvent.h"
#include "Tauola/TauolaParticle.h"
#include "HepMC3/GenEvent.h"

namespace Tauolapp
{

class TauolaHepMC3Event : public TauolaEvent {
//
// Constructors
//
public:
    /** Constructor which keeps a pointer to the HepMC3::GenEvent*/
    TauolaHepMC3Event(HepMC3::GenEvent *event);

    ~TauolaHepMC3Event();
//
// Functions
//
public:

    /** Implementation of TauolaEvent virtual method.
      This returns a list of particles in the event with
      pdg id = "pdgID". */
    std::vector<TauolaParticle*> findParticles(int pdgID);

    /** Implementation of TauolaEven virtual method.
      This returns a list of particles in the event with
      pdg id = "pdgID" and stable status code. */
    std::vector<TauolaParticle*> findStableParticles(int pdgID);

    /** Overriding of TauolaEvent decayEndgame method.
      Converts the momentum and length units */
    void eventEndgame();

//
// Accessors
//
public:
    HepMC3::GenEvent* getEvent()                   { return m_event;           }       //!< Get HepMC3 event

    HepMC3::GenParticle& new_particle()            { return m_event->new_particle(); } //!< Create new particle
    HepMC3::GenVertex&   new_vertex()              { return m_event->new_vertex();   } //!< Create new vertex

    int  last_version()                            { return m_event->last_version(); } //!< Get last version of HepMC3 event
    void delete_vertex(HepMC3::GenVertex *v)       { m_event->delete_vertex(*v); }     //!< Delete HepMC3 vertex

//
// Fields
//
private:
    HepMC3::GenEvent             *m_event;         //!< HepMC3 event
    std::vector<TauolaParticle*>  m_tau_list;      //!< List of particles to be decayed
    string                        m_momentum_unit; //!< Momentum unit name
    string                        m_length_unit;   //!< Length unit name
};

} // namespace Tauolapp
#endif
