#ifndef  HEPMC_DATA_GENPARTICLEDATA_H
#define  HEPMC_DATA_GENPARTICLEDATA_H
/**
 *  @file GenParticleData.h
 *  @brief Definition of \b class GenParticleData
 *
 *  @struct HepMC::GenParticleData
 *  @brief Stores serializable particle information
 *
 *  @ingroup data
 *
 */
#include "HepMC/FourVector.h"

namespace HepMC {

// NOTE: Keep in mind the data alignment
//       Currently it's 8b alignment = 56b total
struct GenParticleData {
    int        pdg_id;            //!< PDG ID
    int        status;            //!< Status
    int        status_subcode;    //!< Status subcode
    bool       is_mass_set;       //!< Check if generated mass is set
    double     mass;              //!< Generated mass (if set)
    FourVector momentum;          //!< Momentum
};

} // namespace HepMC

#endif