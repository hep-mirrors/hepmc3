#ifndef  HEPMC3_DATA_GENVERTEXDATA_H
#define  HEPMC3_DATA_GENVERTEXDATA_H
/**
 *  @file GenVertexData.h
 *  @brief Definition of \b class HepMC3::GenVertexData
 *
 *  @struct HepMC3::GenVertexData
 *  @brief Stores vertex information
 *
 *  @ingroup data
 *
 *  @struct HepMC3::GenVertexSerializableData
 *  @brief Stores serializable vertexe information
 *
 *  @ingroup data
 */
#include "HepMC3/FourVector.h"
#include "HepMC3/Setup.h"

#include <vector>

namespace HepMC3 {

class GenEvent;
class GenParticle;

struct GenVertexData {
    GenEvent                 *event;         //!< Parent event
    unsigned int              index;         //!< Index
    FourVector                position;      //!< Position in time-space
    std::vector<GenParticle>  particles_in;  //!< Incoming particle list
    std::vector<GenParticle>  particles_out; //!< Outgoing particle list
};

struct GenVertexSerializableData {
    FourVector position; //!< Position in time-space
};

} // namespace HepMC3

#endif
