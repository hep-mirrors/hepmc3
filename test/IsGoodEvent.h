//////////////////////////////////////////////////////////////////////////
// IsGoodEvent.h
//
// garren@fnal.gov, May 2007
// andrii.verbytskyi@mpp.mpg.de, Nov 2018
//
//////////////////////////////////////////////////////////////////////////

//! used in the tests

/// \class  IsGoodEvent
/// event selection predicate. returns true if the event contains
/// a photon with pT > 50 GeV
class IsGoodEvent {
public:
    bool operator()( const HepMC::GenEvent& evt ) { 
	FOREACH( const HepMC::GenParticlePtr &p, evt.particles() )
	if ( p->pdg_id() == 22 && p->momentum().perp() > 25. ) return 1;
	return 0;
    }
};
/// \class  IsGoodEventDIS
/// event selection predicate. returns true if the event contains
/// an electron with E > 10 GeV
class IsGoodEventDIS {
public:
    bool operator()( const HepMC::GenEvent& evt ) { 
	FOREACH( const HepMC::GenParticlePtr &p, evt.particles() )
	if ( p->status() == 1 &&  std::abs(p->pdg_id()) == 11 && p->momentum().e() > 10.) return 1;
	return 0;
    }
};