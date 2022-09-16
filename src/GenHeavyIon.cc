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
    double spectator_neutrons, spectator_protons, eccentricity;
#endif
    const char * buff = att.c_str();
    int version = 0;
    const char * data = buff;
    if (att.size() > 1 && att.at(0) == 'v' && att.at(1) == '0' ) {
        version = 0;
        data = buff + 2;
    }
    if (att.size() > 1 && att.at(0) == 'v' && att.at(1) == '1' ) {
        version = 1;
        data = buff + 2;
    }
    int nlen = 0;
    int result = -1;
    if ( version == 0) {
        /* The counting of characters requires C99*/
        result = sscanf(data,"%i %i %i %i %i %i %i %i %i %f %f %f %f %f %f %i %i %i %i%n",
                        &Ncoll_hard, &Npart_proj, &Npart_targ, &Ncoll, //%i %i %i %i
                        &spectator_neutrons, &spectator_protons, //%i %i
                        &N_Nwounded_collisions, &Nwounded_N_collisions, &Nwounded_Nwounded_collisions, &impact_parameter, &event_plane_angle, //%i %i %i %f %f
                        &eccentricity, //%f
                        &sigma_inel_NN, &centrality, &user_cent_estimate, &Nspec_proj_n, &Nspec_targ_n, &Nspec_proj_p, &Nspec_targ_p // %f %f %f %i %i %i %i
                        ,&nlen
                       );
        if (result != 19) return false;
    }
    if ( version == 1) {
        /* The counting of characters requires C99*/
        result = sscanf(data,"%i %i %i %i %i %i %f %f %f %f %f %i %i %i %i%n",
                        &Ncoll_hard, &Npart_proj, &Npart_targ, &Ncoll,  //%i %i %i %i
                        &N_Nwounded_collisions, &Nwounded_N_collisions, &Nwounded_Nwounded_collisions, &impact_parameter, &event_plane_angle, //%i %i %i %f %f
                        &sigma_inel_NN, &centrality, &user_cent_estimate, &Nspec_proj_n, &Nspec_targ_n, &Nspec_proj_p, &Nspec_targ_p // %f %f %f %i %i %i %i
                        ,&nlen
                       );
        if (result != 16) return false;
    }

    data+=nlen;

    int N=0;
    double dval;
    int ival;
    if (sscanf(data," %i%n",&N, &nlen)!=1) return false;
    data+=nlen;
    for ( int i = 0; i < N; ++i )
    {
        if (sscanf(data," %i %f%n",&ival, &dval, &nlen)!=2) return false;
        participant_plane_angles[ival]=dval;
        data+=nlen;
    }
    if (sscanf(data," %i%n",&N, &nlen)!=1) return false;
    data+=nlen;
    for ( int i = 0; i < N; ++i )
    {
        if (sscanf(data," %i %f%n",&ival, &dval, &nlen)!=2) return false;
        eccentricities[ival]=dval;
        data+=nlen;
    }

    return true;

    /*


        std::istringstream is(att);
        std::string version;

        if ( att[0]  != 'v' ) {
            is >> Ncoll_hard >> Npart_proj >> Npart_targ >> Ncoll
               >> spectator_neutrons >> spectator_protons
               >> N_Nwounded_collisions >> Nwounded_N_collisions
               >> Nwounded_Nwounded_collisions >> impact_parameter
               >> event_plane_angle >> eccentricity >> sigma_inel_NN
               >> centrality;
            return !is.fail();
        } else
            is >> version;



        is >> Ncoll_hard >> Npart_proj >> Npart_targ >> Ncoll;
        if ( version == "v0" ) is >> spectator_neutrons >> spectator_protons;
        is >> N_Nwounded_collisions >> Nwounded_N_collisions
           >> Nwounded_Nwounded_collisions >> impact_parameter
           >> event_plane_angle;
        if ( version == "v0" ) is >> eccentricity;
        is >> sigma_inel_NN >> centrality;
        if ( version != "v0" ) is >> user_cent_estimate;
        is >> Nspec_proj_n >> Nspec_targ_n >> Nspec_proj_p >> Nspec_targ_p;

        int N, ord;
        is >> N;
        for ( int i = 0; i < N; ++i ) {
            is >> ord;
            is >> participant_plane_angles[ord];
        }
        is >> N;
        for ( int i = 0; i < N; ++i ) {
            is >> ord;
            is >> eccentricities[ord];
        }

        return !is.fail();
    */
}

bool GenHeavyIon::to_string(std::string &att) const {

    /** The ANSI C standard, in section 7.19.6.1, says this about the f format specifier 6 digits
     *  But the actuall safe value should be able to carry '-infinity', i.e. 9
     *  int: 12 characters
     **/
    const size_t isize = 12;
    const size_t fsize = 9;
    att.reserve(3+13*isize+6*fsize+18+1+isize+(2+isize+fsize)*participant_plane_angles.size()+1+isize+(2+isize+fsize)*eccentricities.size());
    char *buffpos = &att[0];
#ifndef HEPMC3_NO_DEPRECATED
    buffpos+=sprintf(buffpos,"%s%i %i %i %i %i %i %i %i %i %f %f %f %f %f %f %i %i %i %i",(!forceoldformat)?"v0 ":"",
                     Ncoll_hard, Npart_proj, Npart_targ, Ncoll, //%i %i %i %i
                     spectator_neutrons, spectator_protons, //%i %i
                     N_Nwounded_collisions, Nwounded_N_collisions, Nwounded_Nwounded_collisions, impact_parameter, event_plane_angle, //%i %i %i %f %f
                     eccentricity, //%f
                     sigma_inel_NN, centrality, user_cent_estimate, Nspec_proj_n, Nspec_targ_n, Nspec_proj_p, Nspec_targ_p // %f %f %f %i %i %i %i
                    );
#else
    buffpos+=sprintf(buffpos,"v1 %i %i %i %i %i %i %f %f %f %f %f %i %i %i %i",
                     Ncoll_hard, Npart_proj, Npart_targ, Ncoll,  //%i %i %i %i
                     N_Nwounded_collisions, Nwounded_N_collisions, Nwounded_Nwounded_collisions, impact_parameter, event_plane_angle, //%i %i %i %f %f
                     sigma_inel_NN, centrality, user_cent_estimate, Nspec_proj_n, Nspec_targ_n, Nspec_proj_p, Nspec_targ_p // %f %f %f %i %i %i %i
                    );
#endif
    buffpos+=sprintf(buffpos, " %i", participant_plane_angles.size());
    for ( auto it: participant_plane_angles) buffpos += sprintf(buffpos, " %i %f", it.first, it.second);
    buffpos+=sprintf(buffpos, " %i", eccentricities.size());
    for ( auto it: eccentricities) buffpos += sprintf(buffpos, " %i %f", it.first, it.second);
    att.resize(buffpos-att.c_str());

//    m_stream->write(buff, buffpos-buff);


    /*
      std::ostringstream os;

    #ifndef HEPMC3_NO_DEPRECATED
      if ( !forceoldformat ) os << "v0 ";
    #else
      os << "v1 ";
    #endif

      os << std::setprecision(8)
         << Ncoll_hard << " " << Npart_proj << " "
         << Npart_targ << " " << Ncoll << " "
    #ifndef HEPMC3_NO_DEPRECATED
         << spectator_neutrons << " " << spectator_protons << " "
    #endif
         << N_Nwounded_collisions << " " << Nwounded_N_collisions << " "
         << Nwounded_Nwounded_collisions << " " << impact_parameter << " "
         << event_plane_angle << " "
    #ifndef HEPMC3_NO_DEPRECATED
         << eccentricity << " "
    #endif
         << sigma_inel_NN << " " << centrality << " " << user_cent_estimate << " "
         << Nspec_proj_n << " " << Nspec_targ_n << " "
         << Nspec_proj_p << " " << Nspec_targ_p << " ";

      os << participant_plane_angles.size();
      for ( std::map<int, double>::const_iterator it = participant_plane_angles.begin();
              it != participant_plane_angles.end(); ++it )
          os << " " << it->first << " " << it->second;

      os << " " << eccentricities.size();
      for ( std::map<int, double>::const_iterator it = eccentricities.begin();
              it != eccentricities.end(); ++it )
          os << " " << it->first << " " << it->second;

      att = os.str();
    */
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
