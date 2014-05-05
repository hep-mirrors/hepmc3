#ifndef  HEPMC3_DATA_GENEVENTSERIALIZABLEDATA_H
#define  HEPMC3_DATA_GENEVENTSERIALIZABLEDATA_H
/**
 *  @file GenEventData.h
 *  @brief Definition of \b class HepMC3::GenEventSerializableData
 *
 *  @struct HepMC3::GenEventSerializableData
 *  @brief Stores serializable event information
 *
 *  @ingroup data
 *
 */
#include <vector>

namespace HepMC3 {

class GenParticleSerializableData;
class GenVertexSerializableData;

struct GenEventData {
    std::vector<GenParticleSerializableData> particles; //!< List of particles
    std::vector<GenVertexSerializableData>   vertices;  //!< List of vertices
    std::vector< std::pair<int,int> >        links;     //!< List of links of incoming/outgoing particles
};

} // namespace HepMC3

#endif
