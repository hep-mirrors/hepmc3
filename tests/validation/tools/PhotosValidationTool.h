#ifndef PHOTOS_VALIDATION_TOOL_H
#define PHOTOS_VALIDATION_TOOL_H

#ifdef PHOTOSPP_HEPMC2
#include "Photos/PhotosHepMCEvent.h"
#include "HepMC/GenEvent.h"
#else
#include "Photos/PhotosHepMC3Event.h"
#include "HepMC3/Search/FindParticles.h"
#include "HepMC3/GenEvent.h"
#endif // ifdef PHOTOSPP_HEPMC2

#include "ValidationTool.h"
#include "Photos/Photos.h"
#include "Photos/Log.h"

#include <cstring> // memset
#include <cstdio> // printf

class PhotosValidationTool : public ValidationTool {
public:
    PhotosValidationTool():m_more_photons_added(0) {
        memset(m_photons_added,0,sizeof(int)*MAX_PHOTONS_TO_KEEP_TRACK_OF);
    }
    
    const std::string name()   { return "Photos++"; }
    bool tool_modifies_event() { return true;       }

    void initialize() {
        Photospp::Photos::initialize();
        Photospp::Photos::setInfraredCutOff(0.001/200);

        HEPMC2CODE( Photospp::Photos::createHistoryEntries(false,3); )
        HEPMC3CODE( Photospp::Photos::createHistoryEntries(true,3);  )
    }

    int process(GenEvent &hepmc) {

        HEPMC2CODE( int buf = -hepmc.particles_size(); )
        HEPMC3CODE( int buf = 0;                          )

        // Process by Photos++
        HEPMC2CODE( Photospp::PhotosHepMCEvent  p_event(&hepmc); )
        HEPMC3CODE( Photospp::PhotosHepMC3Event p_event(&hepmc); )

        p_event.process();

        // Check number of photons created
        HEPMC2CODE( buf += hepmc.particles_size(); )
        HEPMC3CODE(
            FindParticles search(hepmc, FIND_ALL, PDG_ID == 22 && VERSION_CREATED == hepmc.last_version() );
            buf = search.results().size();
        )

        if(buf<MAX_PHOTONS_TO_KEEP_TRACK_OF) ++m_photons_added[buf];
        else                                 ++m_more_photons_added;

        return 0;
    }

    void finalize() {
        Photospp::Log::Summary();
    
        int sum = m_more_photons_added;
        for(int i=0;i<MAX_PHOTONS_TO_KEEP_TRACK_OF; ++i) sum += m_photons_added[i];

        if( sum == 0 ) sum = 1;

        printf("---------------------------------------------------\n");
        printf(" Number of photons added by Photos++ (per event):\n");
        printf("---------------------------------------------------\n");
        for(int i=0;i<MAX_PHOTONS_TO_KEEP_TRACK_OF; ++i) {
            printf("%5i: %7i events (%6.2f%%)\n",i,m_photons_added[i],m_photons_added[i]*100./sum );
        }
        printf(" more: %7i events (%6.2f%%)\n",m_more_photons_added,m_more_photons_added*100./sum );
        printf("total: %7i events\n",sum );
        printf("---------------------------------------------------\n");
    }

private:
    static const int MAX_PHOTONS_TO_KEEP_TRACK_OF = 4;
    int  m_photons_added[MAX_PHOTONS_TO_KEEP_TRACK_OF];
    int  m_more_photons_added;
};

#endif
