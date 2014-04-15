#ifndef  HEPMC3_DATA_GENEVENTDATA_H
#define  HEPMC3_DATA_GENEVENTDATA_H
/**
 *  @file GenEventData.h
 *  @brief Definition of \b class HepMC3::GenEventData
 *
 *  @struct HepMC3::GenEventData
 *  @brief Stores serializable event information
 *  
 *  @ingroup data
 *  
 *  @struct HepMC3::GenEventVersionInfo
 *  @brief Stores serializable information about the event versions
 *  
 *  @ingroup data
 *  
 */
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/DataList.h"

#include <vector>
#include <string>

namespace HepMC3 {

struct GenEventVersionInfo {
    std::string  name;                  //!< Name of the version or tool that created this version
    unsigned int first_particle_index;  //!< Index of the first particle belonging to this version
    unsigned int first_vertex_index;    //!< Index of the first vertex belonging to this version
};

struct GenEventData {
    int                              event_number;  //!< Event number

    std::vector<GenEventVersionInfo> versions;      //!< Version information
    std::vector<std::pair<int,int> > version_links; //!< Links to previous versions of the particles
    DataList<GenParticleData,8>      particle_data; //!< Particle data
    DataList<GenVertexData,8>        vertex_data;   //!< Vertex data
};

} // namespace HepMC3

#endif
