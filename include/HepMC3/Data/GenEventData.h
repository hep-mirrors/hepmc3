#ifndef  HEPMC3_DATA_GENEVENTDATA_H
#define  HEPMC3_DATA_GENEVENTDATA_H
/**
 *  @file GenEventData.h
 *  @brief Definition of \b struct GenEventData
 *
 *  @struct HepMC3::GenEventData
 *  @brief Stores serializable event information
 *
 *  @ingroup data
 *
 */
#include <vector>
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenVertexData.h"

namespace HepMC3 {

struct GenEventData {
    std::vector<GenParticleData> particles; //!< List of particles
    std::vector<GenVertexData>   vertices;  //!< List of vertices
};

} // namespace HepMC3

#endif
