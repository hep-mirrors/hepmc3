/**
 *  @file PhotosHepMC3Event.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Event
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b> 16 April 2014 </b>
 */
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/Log.h"

#include "HepMC3/Search/FindParticles.h"

#include <vector>

#include <boost/foreach.hpp>

namespace Photospp
{

PhotosHepMC3Event::PhotosHepMC3Event(HepMC3::GenEvent *event):
m_event(event) {

    if(!event) return;

    // Find all particles that are not stable and were not deleted in the last version
    HepMC3::FindParticles search( *event, HepMC3::FIND_ALL, HepMC3::STATUS != 1 && HepMC3::VERSION_DELETED > event->last_version() );

    m_particles.reserve(search.results().size());

    BOOST_FOREACH( HepMC3::GenParticle *p, search.results() ) {
        PhotosHepMC3Particle *particle = new PhotosHepMC3Particle(p);
        particle->set_parent_event(this);
        m_particles.push_back( (PhotosParticle*) particle);
    }

    if(Photos::isCreateHistoryEntries) {
        // Create new version
        event->new_version("Photos++");
    }
}

PhotosHepMC3Event::~PhotosHepMC3Event() {

    BOOST_FOREACH( PhotosParticle *p, m_particles ) {
        if(p) delete p;
    }
}

} // namespace Photospp
