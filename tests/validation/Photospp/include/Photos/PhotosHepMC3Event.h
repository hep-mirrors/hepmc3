#ifndef _PhotosHepMC3Event_h_included_
#define _PhotosHepMC3Event_h_included_
/**
 *  @file PhotosHepMC3Event.h
 *  @brief Definition of \b class Photospp::PhotosHepMC3Event
 *
 *  @class PhotosHepMC3Event
 *  @brief Interface to HepMC3::GenEvent objects
 *
 *  @implements Photospp::PhotosEvent
 *
 *  @date Created       <b> 31 March 2014 </b>
 *  @date Last modified <b>  3 April 2014 </b>
 */
#include <vector>
#include "HepMC3/GenEvent.h"
#include "Photos/PhotosEvent.h"
#include "Photos/PhotosParticle.h"

namespace Photospp
{

class PhotosHepMC3Event : public PhotosEvent
{
//
// Constructors
//
public:
    /** Default constructor
     *  Fills particles list
     */
    PhotosHepMC3Event(HepMC3::GenEvent * event);
    /** Default destructor */
    ~PhotosHepMC3Event();

//
// Functions
//
public:
    /** Print HepMC3 event */
    void print()                                   { if(!m_event) return; m_event->print(); }
//
// Accessors
//
public:
    void add_particle(HepMC3::GenParticle *p)      { m_event->add_particle(p); } //!< Add particle to HepMC3 event
    void add_vertex(HepMC3::GenVertex *v)          { m_event->add_vertex(v);   } //!< Add vertex to HepMC3 event

    int  last_version()                            { return m_last_version; } //!< Get last version of HepMC3 event
    std::vector<PhotosParticle*> getParticleList() { return m_particles; }          //!< Get particle list
//
// Fields
//
private:
    HepMC3::GenEvent              *m_event;        //!< HepMC3 event pointer
    int                            m_last_version; //!< Last version of HepMC3 event
    std::vector<PhotosParticle *>  m_particles;    //!< List of particles
};

} // namespace Photospp
#endif
