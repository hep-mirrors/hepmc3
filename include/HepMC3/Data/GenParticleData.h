#ifndef  HEPMC3_DATA_GENPARTICLEDATA_H
#define  HEPMC3_DATA_GENPARTICLEDATA_H
/**
 *  @file GenParticleData.h
 *  @brief Definition of \b class HepMC3::GenParticleData
 *
 *  @struct HepMC3::GenParticleData
 *  @brief Stores serializable particle information
 *  
 *  @ingroup data
 *  
 */
#include "HepMC3/FourVector.h"

#include <iostream>

namespace HepMC3 {

struct GenParticleData {
    int          pdg_id;            //!< PDG ID
    int          production_vertex; //!< Production vertex
    int          end_vertex;        //!< End vertex
    FourVector   momentum;          //!< Momentum
    int          status;            //!< Status
    int          status_subcode;    //!< Status subcode
    double       mass;              //!< Generated mass (if set)
    bool         is_mass_set;       //!< Check if generated mass is set

    /** Print particle data content */
    void print() const {
        std::cout<<pdg_id
                 <<" "<<production_vertex
                 <<" "<<end_vertex
                 <<" "<<momentum.px()
                 <<" "<<momentum.py()
                 <<" "<<momentum.pz()
                 <<" "<<momentum.e()
                 <<" "<<status
                 <<" "<<status_subcode
                 <<" "<<mass
                 <<" "<<is_mass_set
                 <<std::endl;
    }
};

} // namespace HepMC3

#endif
