/**
 *  @file PhotosHepMC3Particle.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Particle
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b>  3 April 2014 </b>
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/Search/FindParticles.h"

#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/Log.h"
#include "Photos/Photos.h"

namespace Photospp
{

PhotosHepMC3Particle::PhotosHepMC3Particle():
m_parent_event(NULL) {
    m_particle = new HepMC3::GenParticle();
}

PhotosHepMC3Particle::PhotosHepMC3Particle(int pdg_id, int status, double mass):
m_parent_event(NULL) {
    m_particle = new HepMC3::GenParticle();
    m_particle->set_pdg_id(pdg_id);
    m_particle->set_status(status);
    m_particle->set_generated_mass(mass);
}

PhotosHepMC3Particle::PhotosHepMC3Particle(HepMC3::GenParticle * particle):
m_parent_event(NULL) {
    m_particle = particle;
}

PhotosHepMC3Particle::~PhotosHepMC3Particle() {
    clear(m_mothers);
    clear(m_daughters);
    clear(m_decay_products);

    // Delete particle if it's not attached to any vertex
    if( false && m_particle &&
       !m_particle->production_vertex() &&
       !m_particle->end_vertex()) delete m_particle;
}

void PhotosHepMC3Particle::setMothers(std::vector<PhotosParticle*> mothers) {

    clear(m_mothers);
    Log::Fatal("PhotosHepMC3Particle::setMothers()??");

    // If there are mothers
    if(mothers.size()>0) {

        HepMC3::GenParticle * part;
        part=dynamic_cast<PhotosHepMC3Particle*>(mothers.at(0))->getHepMC();

        // Use end vertex of first mother as production vertex for particle
        HepMC3::GenVertex *production_vertex = part->end_vertex();
        HepMC3::GenVertex *orig_production_vertex = production_vertex;

        if(!production_vertex) Log::Fatal("PhotosHepMC3Particle::setMothers(): no end_vertex??",1);

        // Loop over all mothers to check that the end points to the right place
        BOOST_FOREACH( PhotosParticle *p, mothers ) {

            HepMC3::GenParticle *moth;
            moth = dynamic_cast<PhotosHepMC3Particle*>(p)->getHepMC();

            if(moth->end_vertex()!=orig_production_vertex)
                Log::Fatal("PhotosHepMC3Particle::setMothers(): Mother production_vertices point to difference places. Can not override. Please delete vertices first.",1);
            else
                production_vertex->add_particle_in(moth);

            // Update status info
            if( moth->status() == PhotosParticle::STABLE )
            moth->set_status(PhotosParticle::DECAYED);
        }
    }
}

void PhotosHepMC3Particle::addDaughter(PhotosParticle* daughter){

    if(!m_particle->end_vertex())
        Log::Fatal("PhotosHepMC3Particle::addDaughters(): attempting to add daughter to particle with no end vertex",1);

    // Add to internal list as well
    m_daughters.push_back(daughter);

    HepMC3::GenParticle * daugh = (dynamic_cast<PhotosHepMC3Particle*>(daughter))->getHepMC();

    m_parent_event->add_particle(daugh);
    m_particle->end_vertex()->add_particle_out(daugh);
}

void PhotosHepMC3Particle::setDaughters(std::vector<PhotosParticle*> daughters){

    clear(m_daughters);
    Log::Fatal("PhotosHepMC3Particle::setDaughters()??");

    // If there are daughters
    if( daughters.size()>0 ) {

        // Use production vertex of first daughter as end vertex for particle
        HepMC3::GenParticle *first_daughter;
        first_daughter = (dynamic_cast<PhotosHepMC3Particle*>(daughters.at(0)))->getHepMC();

        HepMC3::GenVertex *end_vertex;
        end_vertex = first_daughter->production_vertex();
        HepMC3::GenVertex *orig_end_vertex = end_vertex;

        // If it does not exist create it
        if(!end_vertex) Log::Fatal("PhotosHepMC3Particle::setDaughters(): no end_vertex??",1);

        // Loop over all daughters to check that the end points to the right place
        BOOST_FOREACH( PhotosParticle *p, daughters ) {

            HepMC3::GenParticle *daug;
            daug = dynamic_cast<PhotosHepMC3Particle*>(p)->getHepMC();

            if(daug->production_vertex()!=orig_end_vertex)
                Log::Fatal("PhotosHepMC3Particle::setDaughters(): Daughter production_vertices point to difference places. Can not override. Please delete vertices first.",4);
            else {
                m_parent_event->add_particle(daug);
                end_vertex->add_particle_out(daug);
            }
        }
    }
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getMothers() {

    if( m_mothers.size() == 0 ) {
        if( m_particle->production_vertex() ) {

            BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->production_vertex()->particles_in() ) {

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

            BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->end_vertex()->particles_out() ) {

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
    HepMC3::FindParticles search( m_particle, HepMC3::FIND_ALL_DESCENDANTS, HepMC3::STATUS == 1 && HepMC3::VERSION_DELETED > m_parent_event->last_version() );

    // Check if nothing changed since last update
    if( m_decay_products.size() == search.results().size() ) return m_decay_products;

    // If changed - add newly created particles
    search.narrow_down( HepMC3::VERSION_CREATED >= m_parent_event->last_version() );

    m_decay_products.reserve( m_decay_products.size() + search.results().size() );

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

    double sumpx = 0, sumpy = 0, sumpz = 0, sume = 0;

    BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->end_vertex()->particles_in() ) {

        if( Photos::isStatusCodeIgnored( p->status()) ) continue;

        sumpx += p->momentum().px();
        sumpy += p->momentum().py();
        sumpz += p->momentum().pz();
        sume  += p->momentum().e();
    }

    BOOST_FOREACH( HepMC3::GenParticle *p, m_particle->end_vertex()->particles_out() ) {

        if( Photos::isStatusCodeIgnored( p->status()) ) continue;

        sumpx -= p->momentum().px();
        sumpy -= p->momentum().py();
        sumpz -= p->momentum().pz();
        sume  -= p->momentum().e();
    }

    if( sqrt( sumpx*sumpx + sumpy*sumpy + sumpz*sumpz + sume*sume) > Photos::momentum_conservation_threshold ) {
        Log::Warning()<<"Momentum not conserved in the vertex:"<<endl;
        Log::RedirectOutput(Log::Warning(false));
        m_particle->end_vertex()->print();
        Log::RevertOutput();
        return false;
    }

    return true;
}

PhotosParticle* PhotosHepMC3Particle::createNewParticle(
                int pdg_id, int status, double mass,
                double px, double py, double pz, double e) {

    PhotosHepMC3Particle *new_particle = new PhotosHepMC3Particle();
    new_particle->set_parent_event(m_parent_event);
    new_particle->setPdgID(pdg_id);
    new_particle->setStatus(status);
    new_particle->setMass(mass);

    HepMC3::FourVector momentum(px,py,pz,e);
    new_particle->m_particle->set_momentum(momentum);

    m_created_particles.push_back(new_particle);
    return (PhotosParticle*)new_particle;
}

void PhotosHepMC3Particle::createHistoryEntry() {

    if(!m_particle->production_vertex()) {
        Log::Warning()<<"PhotosHepMC3Particle::createHistoryEntry(): particle without production vertex."<<endl;
        return;
    }

    HepMC3::GenParticle *part = new HepMC3::GenParticle(*m_particle);
    part->set_status(Photos::historyEntriesStatus);

    HepMC3::GenVertex *v = m_particle->production_vertex();
    if(!v) { cout<<"err313"<<endl; return; }
    if( v) {
        m_parent_event->add_particle(part);
        v->add_particle_out(part);
    }

}

void PhotosHepMC3Particle::createSelfDecayVertex(PhotosParticle *out) {
    if(!m_particle) return;

    if(m_particle->end_vertex()) {
        Log::Error()<<"PhotosHepMC3Particle::createSelfDecayVertex: particle already has end vertex!"<<endl;
        return;
    }

    // Add new vertex and new particle to HepMC3
    HepMC3::GenParticle *outgoing = new HepMC3::GenParticle( *(dynamic_cast<PhotosHepMC3Particle*>(out)->m_particle) );
    HepMC3::GenVertex   *v        = new HepMC3::GenVertex();

    m_parent_event->add_vertex(v);
    m_parent_event->add_particle(outgoing);

    v->add_particle_in (m_particle);
    v->add_particle_out(outgoing);

    // If this particle was stable, set its status to 2
    if(getStatus()==1) setStatus(2);
}

void PhotosHepMC3Particle::clear(std::vector<PhotosParticle*> &vector) {
    BOOST_FOREACH( PhotosParticle *p, vector ) {
        delete p;
    }
}

} // namespace Photospp
