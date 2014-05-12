#ifndef _PhotosHepMC3Event_h_included_
#define _PhotosHepMC3Event_h_included_

/**
 * @class PhotosHepMC3Event
 *
 * @brief Interface to HepMC3::GenEvent objects
 *
 * This class implements the virtual methods of
 * PhotosEvent. In this way it provides an
 * interface between the generic PhotosEvent class
 * and a HepMC3::GenEvent object.
 *
 * @author Nadia Davidson
 * @date 17 June 2008
 *
 * This code is licensed under GNU General Public Licence.
 * For more informations, see: http://www.gnu.org/licenses/
 */

#include <vector>
#include "HepMC3/GenEvent.h"
#include "Photos/PhotosEvent.h"
#include "Photos/PhotosParticle.h"

namespace Photospp
{

class PhotosHepMC3Event : public PhotosEvent
{
public:
        ~PhotosHepMC3Event();

        /** Constructor which keeps a pointer to the HepMC3::GenEvent*/
        PhotosHepMC3Event(HepMC3::GenEvent * event);

        /** Returns the HepMC3::GenEvent */
        HepMC3::GenEvent * getEvent();

        /** Returns the list of particles */
        std::vector<PhotosParticle*> getParticleList();

        /** Prints event summary */
        void print();
private:
        /** The event */
        HepMC3::GenEvent * m_event;
        /** Particle list */
        std::vector<PhotosParticle *> particles;
};

} // namespace Photospp
#endif
