#include "Tauola/TauolaHepMC3Particle.h"
#include "Tauola/TauolaHepMC3Event.h"
#include "Tauola/f_Decay.h"
#include "Tauola/Log.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/Search/FindParticles.h"

namespace Tauolapp
{

TauolaHepMC3Particle::TauolaHepMC3Particle():
m_parent_event(NULL) {
    m_particle = new HepMC3::GenParticle();
}

TauolaHepMC3Particle::~TauolaHepMC3Particle() {

    BOOST_FOREACH( TauolaParticle *p, m_mothers ) {
        delete p;
    }

    BOOST_FOREACH( TauolaParticle *p, m_daughters ) {
        delete p;
    }

    BOOST_FOREACH( TauolaParticle *p, m_created_particles ) {
        TauolaHepMC3Particle *temp = dynamic_cast<TauolaHepMC3Particle*>(p);
        if( temp->getHepMC3() && temp->getHepMC3()->barcode()==0 ) delete temp->getHepMC3();
        delete p;
    }
}

TauolaHepMC3Particle::TauolaHepMC3Particle(HepMC3::GenParticle * particle) {
    m_particle = particle;
}

HepMC3::GenParticle* TauolaHepMC3Particle::getHepMC3() {
    return m_particle;
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

        //If production_vertex does not exist - create it
        //If it's tau decay - set the time and position including the tau lifetime correction
        //otherwise - copy the time and position of decaying particle
        if(!production_vertex){
            production_vertex = new HepMC3::GenVertex();
            m_parent_event->add_vertex(production_vertex);
        }

        // Loop over all mothers to check that the end points to the right place
        BOOST_FOREACH( TauolaParticle *p, mothers ) {

            HepMC3::GenParticle *moth = dynamic_cast<TauolaHepMC3Particle*>(p)->getHepMC3();

            if( moth->end_vertex() != orig_production_vertex )
                Log::Fatal("Mother production_vertices point to difference places. Can not override. Please delete vertices first.",1);
            else if( !moth->end_vertex() )
                production_vertex->add_particle_in(moth);

            // Update status info
            if( moth->status() == TauolaParticle::STABLE ) {
                moth->set_status(TauolaParticle::DECAYED);
            }
        }
        m_parent_event->add_particle(m_particle);
        production_vertex->add_particle_out(m_particle);
    }
}

void TauolaHepMC3Particle::setDaughters(vector<TauolaParticle*> daughters){

    if(daughters.size()==0) return;

    Log::Fatal( "setDaughters should not be used" );
}

std::vector<TauolaParticle*> TauolaHepMC3Particle::getMothers() {

    if( m_mothers.size() != 0 ) return m_mothers;

    HepMC3::FindParticles search( m_particle, HepMC3::FIND_MOTHERS );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {
        TauolaHepMC3Particle *pp = new TauolaHepMC3Particle(p);
        pp->set_parent_event(m_parent_event);
        m_mothers.push_back( pp );
    }

    return m_mothers;
}

std::vector<TauolaParticle*> TauolaHepMC3Particle::getDaughters() {

    if( m_daughters.size() != 0 ) return m_daughters;

    HepMC3::FindParticles search( m_particle, HepMC3::FIND_DAUGHTERS );

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

    double sumpx = 0, sumpy = 0, sumpz = 0, sume = 0;

    BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->end_vertex()->particles_in() ) {

        sumpx += p->momentum().px();
        sumpy += p->momentum().py();
        sumpz += p->momentum().pz();
        sume  += p->momentum().e();
    }

    BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->end_vertex()->particles_out() ) {

        sumpx -= p->momentum().px();
        sumpy -= p->momentum().py();
        sumpz -= p->momentum().pz();
        sume  -= p->momentum().e();
    }

    if( sqrt( sumpx*sumpx + sumpy*sumpy + sumpz*sumpz + sume*sume) > Tauola::momentum_conservation_threshold ) {
        Log::Warning()<<"Momentum not conserved in the vertex:"<<endl;
        Log::RedirectOutput(Log::Warning(false));
        m_particle->end_vertex()->print();
        Log::RevertOutput();
        return;
    }

    return;
}

// Set (X,T) Position of tau decay trees
void TauolaHepMC3Particle::decayEndgame(){
/* NO POSITION YET
  double lifetime = Tauola::tau_lifetime * (-log( Tauola::randomDouble() ));
  HepMC3::FourVector tau_momentum = m_particle->momentum();

  double mass     = sqrt(abs(  tau_momentum.e()*tau_momentum.e()
                             - tau_momentum.px()*tau_momentum.px()
                             - tau_momentum.py()*tau_momentum.py()
                             - tau_momentum.pz()*tau_momentum.pz()
                            ) );

  // Get previous position
  HepMC3::FourVector previous_position = m_particle->production_vertex()->position();

  // Calculate new position
  HepMC3::FourVector new_position(previous_position.x()+tau_momentum.px()/mass*lifetime,
                                 previous_position.y()+tau_momentum.py()/mass*lifetime,
                                 previous_position.z()+tau_momentum.pz()/mass*lifetime,
                                 previous_position.t()+tau_momentum.e() /mass*lifetime);

  // Set new position
  m_particle->end_vertex()->set_position(new_position);
*/
}

TauolaHepMC3Particle * TauolaHepMC3Particle::createNewParticle(
                        int pdg_id, int status, double mass,
                        double px, double py, double pz, double e) {

    TauolaHepMC3Particle * new_particle = new TauolaHepMC3Particle();
    new_particle->set_parent_event(m_parent_event);
    new_particle->getHepMC3()->set_pdg_id(pdg_id);
    new_particle->getHepMC3()->set_status(status);
    new_particle->getHepMC3()->set_generated_mass(mass);

    HepMC3::FourVector momentum(px,py,pz,e);
    new_particle->getHepMC3()->set_momentum(momentum);

    m_created_particles.push_back(new_particle);

    return new_particle;
}

} // namespace Tauolapp
