#ifndef  HEPMC_DATA_GENEVENTDATA_H
#define  HEPMC_DATA_GENEVENTDATA_H
/**
 *  @file GenEventData.h
 *  @brief Definition of \b struct GenEventData
 *
 *  @struct HepMC::GenEventData
 *  @brief Stores serializable event information
 *
 *  @ingroup data
 *
 */
#include <vector>
#include "HepMC/Data/GenParticleData.h"
#include "HepMC/Data/GenVertexData.h"
#include "HepMC/Units.h"

namespace HepMC {

struct GenEventData {
    int                 event_number;  //!< Event number
    Units::MomentumUnit momentum_unit; //!< Momentum unit
    Units::LengthUnit   length_unit;   //!< Length unit

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

} // namespace HepMC

#endif
