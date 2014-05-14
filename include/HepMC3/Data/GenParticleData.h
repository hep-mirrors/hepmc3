#ifndef  HEPMC3_DATA_GENPARTICLEDATA_H
#define  HEPMC3_DATA_GENPARTICLEDATA_H
/**
 *  @file GenParticleData.h
 *  @brief Definition of \b class GenParticleData
 *
 *  @struct HepMC3::GenParticleData
 *  @brief Stores serializable particle information
 *
 *  @ingroup data
 *
 */
#include "HepMC3/FourVector.h"

namespace HepMC3 {

struct GenParticleData {
    int        pdg_id;            //!< PDG ID
    FourVector momentum;          //!< Momentum
    int        status;            //!< Status
    int        status_subcode;    //!< Status subcode
    double     mass;              //!< Generated mass (if set)
    bool       is_mass_set;       //!< Check if generated mass is set

    int        production_vertex; //!< Production vertex
    int        end_vertex;        //!< End vertex
};

} // namespace HepMC3

#endif
