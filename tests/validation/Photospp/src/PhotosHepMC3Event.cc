/**
 *  @file PhotosHepMC3Event.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Event
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b>  3 April 2014 </b>
 */
#include <boost/foreach.hpp>
#include <vector>
#include "HepMC3/Search/FindParticles.h"

#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/Log.h"

namespace Photospp
{

PhotosHepMC3Event::PhotosHepMC3Event(HepMC3::GenEvent *event):
m_event(event) {

    if(!event) return;

    m_last_version = event->last_version();

    // Find all particles that are not stable and were not deleted in the last version
    HepMC3::FindParticles search( event, HepMC3::FIND_ALL, HepMC3::STATUS != 1 && HepMC3::VERSION_DELETED > m_last_version );

    m_particles.reserve(search.results().size());

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {
        PhotosHepMC3Particle *particle = new PhotosHepMC3Particle(p);
        particle->set_parent_event(this);
        m_particles.push_back( (PhotosParticle*) particle);
    }

    // Create new version
    event->create_new_version("Photos++");
}

PhotosHepMC3Event::~PhotosHepMC3Event() {

    BOOST_FOREACH( PhotosParticle *p, m_particles ) {
        if(p) delete p;
    }
}

} // namespace Photospp
