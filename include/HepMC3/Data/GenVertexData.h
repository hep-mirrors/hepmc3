#ifndef  HEPMC3_DATA_GENVERTEXDATA_H
#define  HEPMC3_DATA_GENVERTEXDATA_H
/**
 *  @file GenVertexData.h
 *  @brief Definition of \b class HepMC3::GenVertexData
 *
 *  @struct HepMC3::GenVertexData
 *  @brief Stores serializable vertex information
 *
 *  @ingroup data
 *
 */
#include "HepMC3/FourVector.h"

namespace HepMC3 {

struct GenVertexData {
    FourVector position; //!< Position in time-space
};

} // namespace HepMC3

#endif
