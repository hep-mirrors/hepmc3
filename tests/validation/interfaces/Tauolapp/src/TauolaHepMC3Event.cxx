#include "Tauola/TauolaHepMC3Particle.h"
#include "Tauola/TauolaHepMC3Event.h"
#include "Tauola/TauolaParticle.h"
#include "Tauola/Log.h"

#include "HepMC3/Search/FindParticles.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenEvent.h"

#include <boost/foreach.hpp>
using namespace std;

namespace Tauolapp
{

TauolaHepMC3Event::TauolaHepMC3Event(HepMC3::GenEvent *event):
m_event(event) {

    // Default units
    m_momentum_unit = "GEV";
    m_length_unit   = "MM";

/* NO UNITS YET
    if(m_event->momentum_unit() != HepMC3::Units::GEV) m_momentum_unit = "MEV";
    if(m_event->length_unit()   != HepMC3::Units::MM ) m_length_unit   = "CM";

    // If needed - change units used by HepMC3 to GEV and MM
    if( m_event->momentum_unit() != HepMC3::Units::GEV ||
      m_event->length_unit()   != HepMC3::Units::MM     )
    {
    m_event->use_units(HepMC3::Units::GEV,HepMC3::Units::MM);
    }
*/
}

TauolaHepMC3Event::~TauolaHepMC3Event(){

    BOOST_FOREACH( TauolaParticle *p, m_tau_list ) {
        delete p;
    }

}

std::vector<TauolaParticle*> TauolaHepMC3Event::findParticles(int pdg_id) {

    Log::Fatal( "findParticles should not be used" );

    return m_tau_list;
}

std::vector<TauolaParticle*> TauolaHepMC3Event::findStableParticles(int pdg_id){

    HepMC3::FindParticles search( *m_event, HepMC3::FIND_ALL, HepMC3::ABS_PDG_ID == pdg_id &&
                                                              !HepMC3::HAS_END_VERTEX &&
                                                              !HepMC3::HAS_SAME_PDG_ID_DAUGHTER );

    BOOST_FOREACH( const HepMC3::GenParticle &p, search.results() ) {

        TauolaHepMC3Particle *pp = new TauolaHepMC3Particle(p);
        pp->set_parent_event(this);
        m_tau_list.push_back( pp );
    }

  return m_tau_list;

}

void TauolaHepMC3Event::eventEndgame(){
/* NO UNITS YET
    //Set output units for the event
    string momentum("GEV"),length("MM");

    switch(Tauola::momentumUnit)
    {
    case Tauola::GEV:
        momentum = "GEV";
        break;
    case Tauola::MEV:
        momentum = "MEV";
        break;
    default:
        momentum = m_momentum_unit;
    }

    switch(Tauola::lengthUnit)
    {
    case Tauola::MM:
        length = "MM";
        break;
    case Tauola::CM:
        length = "CM";
        break;
    default:
        length = m_length_unit;
    }

    m_event->use_units(momentum,length);
*/
}

} // namespace Tauolapp
