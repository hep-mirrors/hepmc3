#ifndef  HEPMC_DATA_GENVERTEXDATA_H
#define  HEPMC_DATA_GENVERTEXDATA_H
/**
 *  @file GenVertexData.h
 *  @brief Definition of \b class GenVertexData
 *
 *  @struct HepMC::GenVertexData
 *  @brief Stores serializable vertex information
 *
 *  @ingroup data
 *
 */
#include "HepMC/FourVector.h"

namespace HepMC {

struct GenVertexData {
    FourVector position; // Position in time-space
};

} // namespace HepMC

#endif
