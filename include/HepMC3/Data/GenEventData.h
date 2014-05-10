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
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

namespace HepMC3 {

struct GenEventData {
    std::vector<GenParticle>          particles; //!< List of particles
    std::vector<GenVertex>            vertices;  //!< List of vertices
    std::vector< std::pair<int,int> > links;     //!< List of links of incoming/outgoing particles
};

} // namespace HepMC3

#endif
