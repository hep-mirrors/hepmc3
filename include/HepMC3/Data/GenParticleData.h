#ifndef  HEPMC3_DATA_GENPARTICLEDATA_H
#define  HEPMC3_DATA_GENPARTICLEDATA_H
/**
 *  @file GenParticleData.h
 *  @brief Definition of \b class HepMC3::GenParticleData
 *
 *  @struct HepMC3::GenParticleData
 *  @brief Stores particle information
 *
 *  @ingroup data
 *
 *  @struct HepMC3::GenParticleSerializableData
 *  @brief Stores serializable particle information
 *
 *  @ingroup data
 *
 */
#include "HepMC3/FourVector.h"

#include <iostream>

#include <boost/weak_ptr.hpp>
using boost::weak_ptr;

namespace HepMC3 {

class GenEvent;
class GenVertexData;

struct GenParticleData {
    GenEvent     *event;          //!< Parent event
    unsigned int  index;          //!< Index
    int           pdg_id;         //!< PDG ID
    FourVector    momentum;       //!< Momentum
    int           status;         //!< Status
    int           status_subcode; //!< Status subcode
    double        mass;           //!< Generated mass (if set)
    bool          is_mass_set;    //!< Check if generated mass is set

    weak_ptr<GenVertexData> production_vertex; //!< Production vertex
    weak_ptr<GenVertexData> end_vertex;        //!< End vertex
};

struct GenParticleSerializableData {
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
