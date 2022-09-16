// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenHeavyIon.cc
 *  @brief Implementation of \b class GenHeavyIon
 *
 */
#include <cstring> // memcmp
#include <cstdlib> // atoi
#include <cstdio> // sprintf
#include <sstream> // sprintf

#include "HepMC3/GenHeavyIon.h"

namespace HepMC3 {

bool GenHeavyIon::from_string(const std::string &att) {
#ifdef HEPMC3_NO_DEPRECATED
    int spectator_neutrons, spectator_protons;
    double eccentricity;
#endif
    const char * buff = att.c_str();
    int version = -1;
    const char * data = buff;
    if (att.size() > 1 && att.at(0) == 'v' && att.at(1) == '0' ) {
        version = 0;
        data = buff + 3;
    }
    if (att.size() > 1 && att.at(0) == 'v' && att.at(1) == '1' ) {
        version = 1;
        data = buff + 3;
    }
    int nlen = 0;
    int result = -1;
    if ( version == -1) {
        /* The counting of characters requires C99*/
        result = sscanf(data,"%i %i %i %i %i %i %i %i %i %lf %lf %lf %lf %lf%n",
                        &Ncoll_hard, &Npart_proj, &Npart_targ, &Ncoll,  //%i %i %i %i
                        &spectator_neutrons, &spectator_protons,   //%i %i
                        &N_Nwounded_collisions,  &Nwounded_N_collisions,  //%i %i
                        &Nwounded_Nwounded_collisions, &impact_parameter,  //%i %lf
                        &event_plane_angle, &eccentricity, &sigma_inel_NN,  //%lf %lf %lf
                        &centrality,        //%lf
                        &nlen
                       );

        if (result != 14) {
            return false;
        }
        return true;
    }
    if ( version == 0) {
        /* The counting of characters requires C99*/
        result = sscanf(data,"%i %i %i %i %i %i %i %i %i %lf %lf %lf %lf %lf %lf %i %i %i %i%n",
                        &Ncoll_hard, &Npart_proj, &Npart_targ, &Ncoll, //%i %i %i %i
                        &spectator_neutrons, &spectator_protons, //%i %i
                        &N_Nwounded_collisions, &Nwounded_N_collisions, &Nwounded_Nwounded_collisions, &impact_parameter, &event_plane_angle, //%i %i %i %lf %lf
                        &eccentricity, //%lf
                        &sigma_inel_NN, &centrality, &user_cent_estimate, &Nspec_proj_n, &Nspec_targ_n, &Nspec_proj_p, &Nspec_targ_p // %lf %lf %lf %i %i %i %i
                        ,&nlen
                       );

        if (result != 19) {
            return false;
        }
    }
    if ( version == 1) {
        /* The counting of characters requires C99*/
        result = sscanf(data,"%i %i %i %i %i %i %lf %lf %lf %lf %lf %i %i %i %i%n",
                        &Ncoll_hard, &Npart_proj, &Npart_targ, &Ncoll,  //%i %i %i %i
                        &N_Nwounded_collisions, &Nwounded_N_collisions, &Nwounded_Nwounded_collisions, &impact_parameter, &event_plane_angle, //%i %i %i %lf %lf
                        &sigma_inel_NN, &centrality, &user_cent_estimate, &Nspec_proj_n, &Nspec_targ_n, &Nspec_proj_p, &Nspec_targ_p // %lf %lf %lf %i %i %i %i
                        ,&nlen
                       );
        if (result != 16) {
            return false;
        }
    }

    data+=nlen;

    int N=0;
    double dval;
    int ival;
    if (sscanf(data," %i%n",&N, &nlen)!=1) return false;
    data+=nlen;
    for ( int i = 0; i < N; ++i )
    {
        if (sscanf(data," %i %lf%n",&ival, &dval, &nlen)!=2) return false;
        participant_plane_angles[ival]=dval;
        data+=nlen;
    }
    if (sscanf(data," %i%n",&N, &nlen)!=1) return false;
    data+=nlen;
    for ( int i = 0; i < N; ++i )
    {
        if (sscanf(data," %i %lf%n",&ival, &dval, &nlen)!=2) return false;
        eccentricities[ival]=dval;
        data+=nlen;
    }

    return true;
}

bool GenHeavyIon::to_string(std::string &att) const {

    /** The ANSI C standard, in section 7.19.6.1, says this about the f format specifier 6 digits
     *  But the actuall safe value should be able to carry '-infinity', i.e. 9
     *  int: 12 characters
     **/
    const size_t isize = 12;
    const size_t fsize = 9;
    const size_t xsize= 3+13*isize+6*fsize+18+1+isize+(2+isize+fsize)*participant_plane_angles.size()+1+isize+(2+isize+fsize)*eccentricities.size();
    att = std::string(xsize,' ');
    char *buffpos = &att[0];
#ifndef HEPMC3_NO_DEPRECATED
    buffpos+=sprintf(buffpos,"%s%i %i %i %i %i %i %i %i %i %lf %lf %lf %lf %lf %lf %i %i %i %i",(!forceoldformat)?"v0 ":"",
                     Ncoll_hard, Npart_proj, Npart_targ, Ncoll, //%i %i %i %i
                     spectator_neutrons, spectator_protons, //%i %i
                     N_Nwounded_collisions, Nwounded_N_collisions, Nwounded_Nwounded_collisions, impact_parameter, event_plane_angle, //%i %i %i %lf %lf
                     eccentricity, //%lf
                     sigma_inel_NN, centrality, user_cent_estimate, Nspec_proj_n, Nspec_targ_n, Nspec_proj_p, Nspec_targ_p // %lf %lf %lf %i %i %i %i
                    );
#else
    buffpos+=sprintf(buffpos,"v1 %i %i %i %i %i %i %lf %lf %lf %lf %lf %i %i %i %i",
                     Ncoll_hard, Npart_proj, Npart_targ, Ncoll,  //%i %i %i %i
                     N_Nwounded_collisions, Nwounded_N_collisions, Nwounded_Nwounded_collisions, impact_parameter, event_plane_angle, //%i %i %i %lf %lf
                     sigma_inel_NN, centrality, user_cent_estimate, Nspec_proj_n, Nspec_targ_n, Nspec_proj_p, Nspec_targ_p // %lf %lf %lf %i %i %i %i
                    );
#endif
    buffpos+=sprintf(buffpos, " %i", participant_plane_angles.size());
    for ( auto it: participant_plane_angles) buffpos += sprintf(buffpos, " %i %lf", it.first, it.second);
    buffpos+=sprintf(buffpos, " %i", eccentricities.size());
    for ( auto it: eccentricities) buffpos += sprintf(buffpos, " %i %lf", it.first, it.second);
    att.resize(buffpos-att.c_str());
    return true;
}


#ifndef HEPMC3_NO_DEPRECATED

bool GenHeavyIon::operator==(const GenHeavyIon& a) const {
    return ( memcmp( (void*) this, (void*) &a, sizeof(class GenHeavyIon) ) == 0 );
}

bool GenHeavyIon::operator!=(const GenHeavyIon& a) const {
    return !( a == *this );
}

void GenHeavyIon::set(const int&nh, const int&np, const int&nt, const int&nc, const int&ns, const int&nsp,
                      const int&nnw, const int&nwn, const int&nwnw,
                      const double& im, const double& pl, const double& ec, const double& s, const double& cent, const double& usrcent) {
    Ncoll_hard                   = nh;
    Npart_proj                   = np;
    Npart_targ                   = nt;
    Ncoll                        = nc;
    spectator_neutrons           = ns;
    spectator_protons            = nsp;
    N_Nwounded_collisions        = nnw;
    Nwounded_N_collisions        = nwn;
    Nwounded_Nwounded_collisions = nwnw;
    impact_parameter             = im;
    event_plane_angle            = pl;
    eccentricity                 = ec;
    sigma_inel_NN                = s;
    centrality                   = cent;
    user_cent_estimate           = usrcent;
}

bool GenHeavyIon::is_valid() const {
    if ( Ncoll_hard                   != 0 ) return true;
    if ( Npart_proj                   != 0 ) return true;
    if ( Npart_targ                   != 0 ) return true;
    if ( Ncoll                        != 0 ) return true;
    if ( spectator_neutrons           != 0 ) return true;
    if ( spectator_protons            != 0 ) return true;
    if ( N_Nwounded_collisions        != 0 ) return true;
    if ( Nwounded_N_collisions        != 0 ) return true;
    if ( Nwounded_Nwounded_collisions != 0 ) return true;
    if ( impact_parameter             != 0 ) return true;
    if ( event_plane_angle            != 0 ) return true;
    if ( eccentricity                 != 0 ) return true;
    if ( sigma_inel_NN                != 0 ) return true;
    if ( centrality                   != 0 ) return true;
    return false;
}

#endif

} // namespace HepMC3
