// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2025 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenHeavyIon.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main() {
    auto HI1  = std::make_shared<GenHeavyIon>();
    HI1->Ncoll_hard = 1;
    HI1->Npart_proj = 2;
    HI1->Npart_targ = 3;
    HI1->Ncoll = 4;
#ifndef HEPMC3_NO_DEPRECATED
    HI1->spectator_neutrons = 5;
    HI1->spectator_protons = 6;
#endif
    HI1->N_Nwounded_collisions = 7;
    HI1->Nwounded_N_collisions = 8;
    HI1->Nwounded_Nwounded_collisions = 9;
    HI1->impact_parameter = 10;
    HI1->event_plane_angle = 11;
#ifndef HEPMC3_NO_DEPRECATED
    HI1->eccentricity = 12;
#endif
    HI1->sigma_inel_NN = 13;
    HI1->centrality = 14;
    HI1->user_cent_estimate = 15;
    HI1->Nspec_proj_n = 16;
    HI1->Nspec_targ_n = 17;
    HI1->Nspec_proj_p = 18;
    HI1->Nspec_targ_p = 19;
    HI1->participant_plane_angles[0] = 20;
    HI1->participant_plane_angles[1] = 21;
    HI1-> eccentricities[0]  = 22;
    HI1-> eccentricities[1]  = 23;

    std::string S1;
    HI1->to_string(S1);
    auto HI2 = std::make_shared<GenHeavyIon>();
    std::string S2;
    HI2->from_string(S1);
    HI2->to_string(S2);
    if (S1 != S2) { printf("1->%s<-\n!=\n1->%s<-\n",S1.c_str(),S2.c_str()); return 1; }

    return 0;
}
