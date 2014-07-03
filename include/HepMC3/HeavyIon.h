#ifndef  HEPMC3_HEAVYION_H
#define  HEPMC3_HEAVYION_H
/**
 *  @file HeavyIon.h
 *  @brief Definition of \b class HeavyIon
 *
 *  @class HepMC3::HeavyIon
 *  @brief Stores additional information about Heavy Ion generator
 *
 *  Creation and use of this information is optional
 *
 */
#include <iostream>

namespace HepMC3 {

class HeavyIon {
//
// Constructors
//
public:
    /** @brief Default constructor */
    HeavyIon();

    /** @brief Constructor with at least 6 first values provided */
    HeavyIon( int nh, int np, int nt, int nc, int ns, int nsp,
              int nnw=0, int nwn=0, int nwnw=0,
              float im=0., float pl=0., float ec=0., float s=0. );

    /** @brief copy constructor */
    HeavyIon( const HeavyIon &orig );

//
// Functions
//
public:
    HeavyIon& operator=( HeavyIon const & rhs );   //!< Assignment operator
    bool      operator==( const HeavyIon& ) const; //!< Operator ==
    bool      operator!=( const HeavyIon& ) const; //!< Operator !=
    bool      is_valid()                    const; //!< Verify that the instance contains non-zero information

    void      print(std::ostream &ostr)     const; //!< Print the contents to output stream
//
// Accessors
//
public:
    int   Ncoll_hard()                      const { return m_Ncoll_hard; }                   //!< Get number of hard scatterings
    void  set_Ncoll_hard(int i)                   { m_Ncoll_hard = i;    }                   //!< Set number of hard scatterings

    int   Npart_proj()                      const { return m_Npart_proj; }                   //!< Get number of projectile participants
    void  set_Npart_proj(int i)                   { m_Npart_proj = i;    }                   //!< Set number of projectile participants

    int   Npart_targ()                      const { return m_Npart_targ; }                   //!< Get number of target participants
    void  set_Npart_targ(int i)                   { m_Npart_targ = i;    }                   //!< Set number of target participants

    int   Ncoll()                           const { return m_Ncoll;      }                   //!< Get number of NN (nucleon-nucleon) collisions
    void  set_Ncoll(int i)                        { m_Ncoll = i;         }                   //!< Set number of NN (nucleon-nucleon) collisions

    int   spectator_neutrons()              const { return m_spectator_neutrons; }           //!< Get number of spectator neutrons
    void  set_spectator_neutrons(int i)           { m_spectator_neutrons = i;    }           //!< Set number of spectator neutrons

    int   spectator_protons()               const { return m_spectator_protons; }            //!< Get number of spectator protons
    void  set_spectator_protons(int i)            { m_spectator_protons = i;    }            //!< Set number of spectator protons

    int   N_Nwounded_collisions()           const { return m_N_Nwounded_collisions; }        //!< Get number of N-Nwounded collisions
    void  set_N_Nwounded_collisions(int i)        { m_N_Nwounded_collisions = i;    }        //!< Set number of N-Nwounded collisions

    int   Nwounded_N_collisions()           const { return m_Nwounded_N_collisions; }        //!< Get number of Nwounded-N collisons
    void  set_Nwounded_N_collisions(int i)        { m_Nwounded_N_collisions = i;    }        //!< Set number of Nwounded-N collisons

    int   Nwounded_Nwounded_collisions()    const { return m_Nwounded_Nwounded_collisions; } //!< Get number of Nwounded-Nwounded collisions
    void  set_Nwounded_Nwounded_collisions(int i) { m_Nwounded_Nwounded_collisions = i;    } //!< Set number of Nwounded-Nwounded collisions

    float impact_parameter()                const { return m_impact_parameter; }             //!< Get impact Parameter(in fm) of collision
    void  set_impact_parameter(float f)           { m_impact_parameter = f;    }             //!< Set Impact Parameter in fm

    float event_plane_angle()               const { return m_event_plane_angle; }            //!< Get azimuthal angle of event plane
    void  set_event_plane_angle(float f)          { m_event_plane_angle = f;    }            //!< Set azimuthal angle of event plane

    float eccentricity()                    const { return m_eccentricity;  }                //!< Get eccentricity of participating nucleons in the transverse plane (as in phobos nucl-ex/0510031)
    void  set_eccentricity(float f)               { m_eccentricity = f;     }                //!< Set eccentricity of participating nucleons in the transverse plane (as in phobos nucl-ex/0510031)

    float sigma_inel_NN()                   const { return m_sigma_inel_NN; }                //!< Get nucleon-nucleon inelastic (including diffractive) cross-section
    void  set_sigma_inel_NN(float f)              { m_sigma_inel_NN = f;    }                //!< Set nucleon-nucleon inelastic (including diffractive) cross-section

//
// Fields
//
private:
    int   m_Ncoll_hard;
    int   m_Npart_proj;
    int   m_Npart_targ;
    int   m_Ncoll;
    int   m_spectator_neutrons;
    int   m_spectator_protons;
    int   m_N_Nwounded_collisions;
    int   m_Nwounded_N_collisions;
    int   m_Nwounded_Nwounded_collisions;
    float m_impact_parameter;
    float m_event_plane_angle;
    float m_eccentricity;
    float m_sigma_inel_NN;
};

} // namespace HepMC3

#endif
