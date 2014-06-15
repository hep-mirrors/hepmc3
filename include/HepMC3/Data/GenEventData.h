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
#include "HepMC3/Units.h"

namespace HepMC3 {

struct GenEventData {
    int                 event_number;
    Units::MomentumUnit momentum_unit;
    Units::LengthUnit   length_unit;

    std::vector<GenParticleData> particles; //!< Particles
    std::vector<GenVertexData>   vertices;  //!< Vertices

    /** @brief First id of the vertex links
     *
     *  If this id is positive - it is the incoming particle id
     *  of a vertex which id is written in GenEventData::links2
     *
     *  If this id is negative, it's the id of a vertex which
     *  outgoing particle id is written in GenEventData::links2
     */
    std::vector<int> links1;
    std::vector<int> links2; //!< Second id of the vertex links
};

} // namespace HepMC3

#endif
