/**
 *  @file PhotosHepMC3Event.cc
 *  @brief Implementation of \b class Photospp::PhotosHepMC3Event
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b> 31 March 2014 </b>
 */
#include <vector>
#include "Photos/PhotosHepMC3Particle.h"
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/Log.h"
using namespace std;

namespace Photospp
{

PhotosHepMC3Event::PhotosHepMC3Event(HepMC3::GenEvent *event):
m_event(event) {

    if(!event) return;

    m_particles.reserve(event->particles().size());

	vector<HepMC3::GenParticle*>::const_iterator i = m_event->particles().begin();
	for( ; i!=m_event->particles().end(); ++i)
	{
		PhotosParticle *particle = new PhotosHepMC3Particle(*i);
		m_particles.push_back(particle);
	}
}

PhotosHepMC3Event::~PhotosHepMC3Event() {

    vector<PhotosParticle*>::const_iterator i  = m_particles.begin();
    for( ; i != m_particles.end(); ++i ) {
        if(*i) delete (*i);
    }
}

} // namespace Photospp
