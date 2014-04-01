/**
 *  @file PhotosHepMC3Particle.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Particle
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b> 31 March 2014 </b>
 */
#include "HepMC3/GenEvent.h"
#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/Log.h"
#include "Photos/Photos.h"

namespace Photospp
{

PhotosHepMC3Particle::PhotosHepMC3Particle() {
    m_particle = new HepMC3::GenParticle();
}

PhotosHepMC3Particle::PhotosHepMC3Particle(int pdg_id, int status, double mass) {
    m_particle = new HepMC3::GenParticle();
    m_particle->set_pdg_id(pdg_id);
    m_particle->set_status(status);
    m_particle->set_generated_mass(mass);
}

PhotosHepMC3Particle::PhotosHepMC3Particle(HepMC3::GenParticle * particle) {
    m_particle = particle;
}

PhotosHepMC3Particle::~PhotosHepMC3Particle() {
    clear(m_mothers);
    clear(m_daughters);

    // Delete particle if it's not attached to any vertex
    if( m_particle &&
       !m_particle->production_vertex() &&
       !m_particle->end_vertex()) delete m_particle;
}

void PhotosHepMC3Particle::setMothers(vector<PhotosParticle*> mothers) {

    clear(m_mothers);

    // If there are mothers
    if(mothers.size()>0) {

        HepMC3::GenParticle * part;
        part=dynamic_cast<PhotosHepMC3Particle*>(mothers.at(0))->getHepMC();

        // Use end vertex of first mother as production vertex for particle
        HepMC3::GenVertex * production_vertex = part->end_vertex();
        HepMC3::GenVertex * orig_production_vertex = production_vertex;

        if(!production_vertex) production_vertex = m_particle->create_production_vertex();
        else                   production_vertex->add_particle_out(m_particle);

        // Loop over all mothers to check that the end points to the right place
        vector<PhotosParticle*>::iterator mother_itr;
        for(mother_itr = mothers.begin(); mother_itr != mothers.end();
            mother_itr++) {

            HepMC3::GenParticle * moth;
            moth = dynamic_cast<PhotosHepMC3Particle*>(*mother_itr)->getHepMC();

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

    if(!m_particle->end_vertex()) m_particle->create_end_vertex();

    // Add to internal list as well
    m_daughters.push_back(daughter);
    
    HepMC3::GenParticle * daugh = (dynamic_cast<PhotosHepMC3Particle*>(daughter))->getHepMC();
    m_particle->end_vertex()->add_particle_out(daugh);
}

void PhotosHepMC3Particle::setDaughters(vector<PhotosParticle*> daughters){

    if(!m_particle->parent_event()) Log::Fatal("PhotosHepMC3Particle::setDaughters(): New particle needs the event set before it's daughters can be added",3);

    clear(m_daughters);

    // If there are daughters
    if( daughters.size()>0 ) {

        // Use production vertex of first daughter as end vertex for particle
        HepMC3::GenParticle *first_daughter;
        first_daughter = (dynamic_cast<PhotosHepMC3Particle*>(daughters.at(0)))->getHepMC();

        HepMC3::GenVertex *end_vertex;
        end_vertex = first_daughter->production_vertex();
        HepMC3::GenVertex *orig_end_vertex = end_vertex;

        // If it does not exist create it
        if(!end_vertex) end_vertex = m_particle->create_end_vertex();
        else            end_vertex->add_particle_in(m_particle);

        // Loop over all daughters to check that the end points to the right place
        vector<PhotosParticle*>::iterator daughter_itr;
        for(daughter_itr  = daughters.begin();
            daughter_itr != daughters.end();
            ++daughter_itr) {

            HepMC3::GenParticle * daug;
            daug = dynamic_cast<PhotosHepMC3Particle*>(*daughter_itr)->getHepMC();


            if(daug->production_vertex()!=orig_end_vertex)
                Log::Fatal("PhotosHepMC3Particle::setDaughters(): Daughter production_vertices point to difference places. Can not override. Please delete vertices first.",4);
            else
                end_vertex->add_particle_out(daug);
        }
    }
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getMothers() {

    // Ignore our new version
    m_particle->parent_event()->set_current_version(m_particle->parent_event()->last_version()-1);

    if( m_mothers.size() == 0 ) {
        if( m_particle->production_vertex() ) {
            vector<HepMC3::GenParticle*>::const_iterator pcle_itr     = m_particle->production_vertex()->particles_in().begin();
            vector<HepMC3::GenParticle*>::const_iterator pcle_itr_end = m_particle->production_vertex()->particles_in().end();

            for(;pcle_itr != pcle_itr_end; pcle_itr++) {
                m_mothers.push_back(new PhotosHepMC3Particle(*pcle_itr));
            }
        }
    }

    // Go back to new version
    m_particle->parent_event()->set_current_version(m_particle->parent_event()->last_version());

    return m_mothers;
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getDaughters() {

    // Ignore our new version
    m_particle->parent_event()->set_current_version(m_particle->parent_event()->last_version()-1);

    if( m_daughters.size() == 0 && m_particle->parent_event() ) {
        if( m_particle->end_vertex() ) {
            for( vector<HepMC3::GenParticle*>::const_iterator i  = m_particle->end_vertex()->particles_out().begin();
                                                              i != m_particle->end_vertex()->particles_out().end();
                                                              ++i ) {
                m_daughters.push_back( new PhotosHepMC3Particle(*i) );
            }
        }
    }

    // Go back to new version
    m_particle->parent_event()->set_current_version(m_particle->parent_event()->last_version());

    return m_daughters;
}

std::vector<PhotosParticle*> PhotosHepMC3Particle::getAllDecayProducts() {

    m_decay_products.clear();

    if( !hasDaughters() ) return m_decay_products;

    std::vector<PhotosParticle*> daughters = getDaughters();

    // copy daughters to list of all decay products
    m_decay_products.insert(m_decay_products.end(),daughters.begin(),daughters.end());

    // Now, get all daughters recursively, without duplicates.
    // That is, for each daughter:
    // 1)  get list of her daughters
    // 2)  for each particle on this list:
    //  a) check if it is already on the list
    //  b) if it's not, add her to the end of the list
    for(unsigned int i=0;i<m_decay_products.size();i++) {
        std::vector<PhotosParticle*> daughters2 = m_decay_products[i]->getDaughters();

        if(!m_decay_products[i]->hasDaughters()) continue;
        for(unsigned int j=0;j<daughters2.size();j++) {
            bool add=true;
            for(unsigned int k=0;k<m_decay_products.size();k++)
            if( daughters2[j]->getBarcode() == m_decay_products[k]->getBarcode() ) {
                add=false;
                break;
            }

            if(add) m_decay_products.push_back(daughters2[j]);
        }
    }

    return m_decay_products;
}

bool PhotosHepMC3Particle::checkMomentumConservation() {
    if(!m_particle) return true;

    if(!m_particle->end_vertex()) return true;

    // HepMC3 version of check_momentum_conservation
    // Omitting history entries (status == 3)

    double sumpx = 0, sumpy = 0, sumpz = 0, sume = 0;

    for( vector<HepMC3::GenParticle*>::iterator part1 = m_particle->end_vertex()->particles_in().begin();
         part1 != m_particle->end_vertex()->particles_in().end();
         ++part1 ) {

    if( Photos::isStatusCodeIgnored((*part1)->status()) ) continue;

        sumpx += (*part1)->momentum().px();
        sumpy += (*part1)->momentum().py();
        sumpz += (*part1)->momentum().pz();
        sume  += (*part1)->momentum().e();
    }

    for( vector<HepMC3::GenParticle*>::iterator part2 = m_particle->end_vertex()->particles_out().begin();
         part2 != m_particle->end_vertex()->particles_out().end();
         ++part2 ) {

        if( Photos::isStatusCodeIgnored((*part2)->status()) ) continue;

        sumpx -= (*part2)->momentum().px();
        sumpy -= (*part2)->momentum().py();
        sumpz -= (*part2)->momentum().pz();
        sume  -= (*part2)->momentum().e();
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
    if(!v) v = m_particle->create_production_vertex();
    if( v) v->add_particle_out(part);

}

void PhotosHepMC3Particle::createSelfDecayVertex(PhotosParticle *out) {
    if(!m_particle) return;

    if(m_particle->end_vertex()) {
        Log::Error()<<"PhotosHepMC3Particle::createSelfDecayVertex: particle already has end vertex!"<<endl;
        return;
    }

    if(m_particle->parent_event()==NULL) {
        Log::Error()<<"PhotosHepMC3Particle::createSelfDecayVertex: particle not in the HepMC3 event!"<<endl;
        return;
    }

    // Add new vertex and new particle to HepMC3
    HepMC3::GenParticle *outgoing = new HepMC3::GenParticle( *(dynamic_cast<PhotosHepMC3Particle*>(out)->m_particle) );
    HepMC3::GenVertex   *v        = new HepMC3::GenVertex();

    v->add_particle_in (m_particle);
    v->add_particle_out(outgoing);

    m_particle->parent_event()->add_vertex(v);

    // If this particle was stable, set its status to 2
    if(getStatus()==1) setStatus(2);
}

void PhotosHepMC3Particle::clear(std::vector<PhotosParticle*> &vector) {
    for( std::vector<PhotosParticle*>::iterator i = vector.begin(); i != vector.end(); ++i) {
        delete (*i);
    }
}

} // namespace Photospp
