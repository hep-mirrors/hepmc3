#include "Tauola/TauolaHepMC3Particle.h"
#include "Tauola/TauolaHepMC3Event.h"
#include "Tauola/f_Decay.h"
#include "Tauola/Log.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/Search/FindParticles.h"

#include <boost/foreach.hpp>

namespace Tauolapp
{

TauolaHepMC3Particle::~TauolaHepMC3Particle() {

    BOOST_FOREACH( TauolaParticle *p, m_mothers ) {
        delete p;
    }

    BOOST_FOREACH( TauolaParticle *p, m_daughters ) {
        delete p;
    }

    BOOST_FOREACH( TauolaParticle *p, m_created_particles ) {
        delete p;
    }
}

TauolaHepMC3Particle::TauolaHepMC3Particle(HepMC3::GenParticle * particle) {
    m_particle = particle;
}

void TauolaHepMC3Particle::undecay() {

    if( m_particle->end_vertex() ) {

        // This will invalidate all downstream
        m_parent_event->delete_vertex( m_particle->end_vertex() );
    }

    m_daughters.clear();

    m_particle->set_status(TauolaParticle::STABLE);
}

void TauolaHepMC3Particle::setMothers(vector<TauolaParticle*> mothers){

    //If there are mothers
    if(mothers.size()>0) {
        HepMC3::GenParticle *part = dynamic_cast<TauolaHepMC3Particle*>(mothers.at(0))->getHepMC3();

        // Use end vertex of first mother as production vertex for particle
        HepMC3::GenVertex *production_vertex = part->end_vertex();
        HepMC3::GenVertex *orig_production_vertex = production_vertex;

        // If production_vertex does not exist - create it
        if(!production_vertex) production_vertex = &m_parent_event->getEvent()->new_vertex();

        // Loop over all mothers to check that the end points to the right place
        BOOST_FOREACH( TauolaParticle *p, mothers ) {

            HepMC3::GenParticle *moth = dynamic_cast<TauolaHepMC3Particle*>(p)->getHepMC3();

            if( moth->end_vertex() != orig_production_vertex )
                Log::Fatal("Mother production_vertices point to difference places. Can not override. Please delete vertices first.",1);
            else if( !moth->end_vertex() )
                production_vertex->add_particle_in(*moth);

            // Update status info
            if( moth->status() == TauolaParticle::STABLE ) {
                moth->set_status(TauolaParticle::DECAYED);
            }
        }
        production_vertex->add_particle_out(*m_particle);
    }
}

void TauolaHepMC3Particle::setDaughters(vector<TauolaParticle*> daughters){

    if(daughters.size()==0) return;

    Log::Fatal( "setDaughters is deprecated and should never be used. Please report this problem." );
}

std::vector<TauolaParticle*> TauolaHepMC3Particle::getMothers() {

    if( m_mothers.size() != 0 ) return m_mothers;

    HepMC3::FindParticles search( *m_particle, HepMC3::FIND_MOTHERS );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {
        TauolaHepMC3Particle *pp = new TauolaHepMC3Particle(p);
        pp->set_parent_event(m_parent_event);
        m_mothers.push_back( pp );
    }

    return m_mothers;
}

std::vector<TauolaParticle*> TauolaHepMC3Particle::getDaughters() {

    if( m_daughters.size() != 0 ) return m_daughters;

    HepMC3::FindParticles search( *m_particle, HepMC3::FIND_DAUGHTERS );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {
        TauolaHepMC3Particle *pp = new TauolaHepMC3Particle(p);
        pp->set_parent_event(m_parent_event);
        m_daughters.push_back( pp );
    }

    return m_daughters;
}

void TauolaHepMC3Particle::checkMomentumConservation() {
    if(!m_particle) return;

    if(!m_particle->end_vertex()) return;

    // HepMC3 version of check_momentum_conservation

    HepMC3::FourVector sum;

    HepMC3::FindParticles search( *m_particle, HepMC3::FIND_PRODUCTION_SIBLINGS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {

        sum += p->momentum();
    }

    HepMC3::FindParticles search2( *m_particle, HepMC3::FIND_DAUGHTERS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    BOOST_FOREACH( HepMC3::GenParticle *p, search2.results() ) {

        sum -= p->momentum();
    }

    if( sum.length() > Tauola::momentum_conservation_threshold ) {
        Log::Warning()<<"Momentum not conserved in the vertex:"<<endl;
        m_particle->end_vertex()->print(Log::Warning(false),1);
    }
}

void TauolaHepMC3Particle::decayEndgame() {
    double lifetime = Tauola::tau_lifetime * (-log( Tauola::randomDouble() ));
    HepMC3::FourVector tau_momentum = m_particle->momentum();

    double mass = fabs( m_particle->generated_mass() );

    // Get previous position
    const HepMC3::FourVector& previous_position = m_particle->production_vertex()->position();

    // Calculate new position
    HepMC3::FourVector new_position(previous_position.x() + tau_momentum.px()/mass * lifetime,
                                    previous_position.y() + tau_momentum.py()/mass * lifetime,
                                    previous_position.z() + tau_momentum.pz()/mass * lifetime,
                                    previous_position.t() + tau_momentum.e() /mass * lifetime);

    // Set new position
    m_particle->end_vertex()->set_position(new_position);

}

TauolaHepMC3Particle * TauolaHepMC3Particle::createNewParticle(
                        int pdg_id, int status, double mass,
                        double px, double py, double pz, double e) {

    HepMC3::GenParticle &hepmc3 = m_parent_event->getEvent()->new_particle( HepMC3::FourVector(px,py,pz,e), pdg_id, status );
    TauolaHepMC3Particle *new_particle = new TauolaHepMC3Particle(&hepmc3);
    new_particle->set_parent_event(m_parent_event);
    new_particle->setMass(mass);

    m_created_particles.push_back(new_particle);

    return new_particle;
}

void TauolaHepMC3Particle::setPx( double px ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPx(px);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void TauolaHepMC3Particle::setPy( double py ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPy(py);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void TauolaHepMC3Particle::setPz( double pz ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPz(pz);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void TauolaHepMC3Particle::setE( double e ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setE(e);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

} // namespace Tauolapp
