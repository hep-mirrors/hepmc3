/**
 *  @file PhotosHepMC3Particle.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Particle
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b> 16 April 2014 </b>
 */
#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/Log.h"
#include "Photos/Photos.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/Search/FindParticles.h"

#include <boost/foreach.hpp>

namespace Photospp
{

PhotosHepMC3Particle::PhotosHepMC3Particle(HepMC3::GenParticle * particle):
m_parent_event(NULL) {
    m_particle = particle;
}

PhotosHepMC3Particle::~PhotosHepMC3Particle() {
    clear(m_mothers);
    clear(m_daughters);
    clear(m_decay_products);
}

void PhotosHepMC3Particle::setMothers(std::vector<PhotosParticle*> mothers) {

    Log::Fatal("PhotosHepMC3Particle::setMothers() is deprecated and should never be used. Please report this problem.");

}

void PhotosHepMC3Particle::addDaughter(PhotosParticle* daughter){

    if(!m_particle->end_vertex())
        Log::Fatal("PhotosHepMC3Particle::addDaughters(): attempting to add daughter to particle with no end vertex",1);

    // Add to internal list as well
    m_daughters.push_back(daughter);

    HepMC3::GenParticle * daugh = (dynamic_cast<PhotosHepMC3Particle*>(daughter))->getHepMC();

    m_particle->end_vertex()->add_particle_out(*daugh);
}

void PhotosHepMC3Particle::setDaughters(std::vector<PhotosParticle*> daughters){

    Log::Fatal("PhotosHepMC3Particle::setDaughters() is deprecated and should never be used. Please report this problem.");
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getMothers() {

    if( m_mothers.size() == 0 ) {
        if( m_particle->production_vertex() ) {

            HepMC3::FindParticles search( *m_particle, HepMC3::FIND_MOTHERS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

            BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {

                PhotosHepMC3Particle *pp = new PhotosHepMC3Particle(p);
                pp->set_parent_event(m_parent_event);
                m_mothers.push_back(pp);
            }
        }
    }

    return m_mothers;
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getDaughters() {

    if( m_daughters.size() == 0 ) {
        if( m_particle->end_vertex() ) {

            HepMC3::FindParticles search( *m_particle, HepMC3::FIND_DAUGHTERS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

            BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {

                PhotosHepMC3Particle *pp = new PhotosHepMC3Particle(p);
                pp->set_parent_event(m_parent_event);
                m_daughters.push_back(pp);
            }
        }
    }

    return m_daughters;
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getAllDecayProducts() {

    // Find all stable decay products that are not deleted
    HepMC3::FindParticles search( *m_particle, HepMC3::FIND_ALL_DESCENDANTS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    // Check if no photons were added since last update
    if( m_decay_products.size() == search.results().size() ) return m_decay_products;

    m_decay_products.clear();
    m_decay_products.reserve( search.results().size() );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {

        PhotosHepMC3Particle *pp = new PhotosHepMC3Particle(p);
        pp->set_parent_event(m_parent_event);
        m_decay_products.push_back(pp);
    }

    return m_decay_products;
}

bool PhotosHepMC3Particle::checkMomentumConservation() {
    if(!m_particle) return true;

    if(!m_particle->end_vertex()) return true;

    // HepMC3 version of check_momentum_conservation
    // Omitting history entries (status == 3)

    HepMC3::FourVector sum;

    HepMC3::FindParticles search( *m_particle, HepMC3::FIND_PRODUCTION_SIBLINGS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {

        if( Photos::isStatusCodeIgnored( p->status()) ) continue;

        sum += p->momentum();
    }

    HepMC3::FindParticles search2( *m_particle, HepMC3::FIND_DAUGHTERS, HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    BOOST_FOREACH( HepMC3::GenParticle *p, search2.results() ) {

        if( Photos::isStatusCodeIgnored( p->status()) ) continue;

        sum -= p->momentum();
    }

    if( sum.length() > Photos::momentum_conservation_threshold ) {
        Log::Warning()<<"Momentum not conserved in the vertex:"<<endl;
        m_particle->end_vertex()->print(Log::Warning(false),1);
        return false;
    }

    return true;
}

PhotosParticle* PhotosHepMC3Particle::createNewParticle(
                int pdg_id, int status, double mass,
                double px, double py, double pz, double e) {

    HepMC3::GenParticle  &hepmc3       = m_parent_event->getEvent()->new_particle( HepMC3::FourVector(px,py,pz,e), pdg_id, status );
    PhotosHepMC3Particle *new_particle = new PhotosHepMC3Particle(&hepmc3);

    new_particle->set_parent_event(m_parent_event);
    new_particle->setMass(mass);

    m_created_particles.push_back(new_particle);
    return (PhotosParticle*)new_particle;
}

void PhotosHepMC3Particle::createHistoryEntry() {

    // Nothing to do here
}

void PhotosHepMC3Particle::createSelfDecayVertex(PhotosParticle *out) {
    if(!m_particle) return;

    if(m_particle->end_vertex()) {
        Log::Error()<<"PhotosHepMC3Particle::createSelfDecayVertex: particle already has end vertex!"<<endl;
        return;
    }

    // Add new vertex and new particle to HepMC3
    HepMC3::GenParticle &outgoing = m_parent_event->getEvent()->new_particle();

    outgoing.set_data( dynamic_cast<PhotosHepMC3Particle*>(out)->m_particle->data() );

    HepMC3::GenVertex &v = m_parent_event->getEvent()->new_vertex();

    v.add_particle_in (*m_particle);
    v.add_particle_out(outgoing);

    // If this particle was stable, set its status to 2
    if(getStatus()==1) setStatus(2);
}


void PhotosHepMC3Particle::setPx( double px ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPx(px);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void PhotosHepMC3Particle::setPy( double py ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPy(py);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void PhotosHepMC3Particle::setPz( double pz ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setPz(pz);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void PhotosHepMC3Particle::setE( double e ) {
    if(!m_particle) return;

    HepMC3::FourVector tmp(m_particle->momentum());
    tmp.setE(e);
    m_particle->set_momentum(tmp);

    // If versioning is used, we have to update pointer
    m_particle = m_particle->last_version();
}

void PhotosHepMC3Particle::clear(std::vector<PhotosParticle*> &vector) {
    BOOST_FOREACH( PhotosParticle *p, vector ) {
        delete p;
    }
}

} // namespace Photospp
