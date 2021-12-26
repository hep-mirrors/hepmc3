// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file HEPEVT_Wrapper.cc
 *  @brief Implementation of sorting functions used to convert HEPEVT block
 */
#include <algorithm>
#include <set>
#include <vector>

#include "HepMC3/HEPEVT_Helpers.h"
namespace HepMC3
{

struct HEPEVT*  hepevtptr;

/** @brief comparison of two particles */
bool GenParticlePtr_greater::operator()(ConstGenParticlePtr lx, ConstGenParticlePtr rx) const
{
    /* Cannot use id as it could be different.*/
    if (lx->pid() != rx->pid()) return (lx->pid() < rx->pid());
    if (lx->status() != rx->status()) return (lx->status() < rx->status());
    /*Hopefully it will reach this point not too often.*/
    return (lx->momentum().e() < rx->momentum().e());
}

/** @brief  Order vertices with equal paths. If the paths are equal, order in other quantities.
 * We cannot use id, as it can be assigned in different way*/
bool pair_GenVertexPtr_int_greater::operator()(const std::pair<ConstGenVertexPtr, int>& lx, const std::pair<ConstGenVertexPtr, int>& rx) const
{
    if (lx.second != rx.second) return (lx.second < rx.second);
    if (lx.first->particles_in().size() != rx.first->particles_in().size()) return (lx.first->particles_in().size() < rx.first->particles_in().size());
    if (lx.first->particles_out().size() != rx.first->particles_out().size()) return (lx.first->particles_out().size() < rx.first->particles_out().size());
    /* The code below is usefull mainly for debug. Assures strong ordering.*/
    std::vector<int> lx_id_in;
    std::vector<int> rx_id_in;
    for (ConstGenParticlePtr pp: lx.first->particles_in()) lx_id_in.push_back(pp->pid());
    for (ConstGenParticlePtr pp: rx.first->particles_in()) rx_id_in.push_back(pp->pid());
    std::sort(lx_id_in.begin(), lx_id_in.end());
    std::sort(rx_id_in.begin(), rx_id_in.end());
    for (unsigned int i = 0; i < lx_id_in.size(); i++) if (lx_id_in[i] != rx_id_in[i]) return  (lx_id_in[i] < rx_id_in[i]);

    std::vector<int> lx_id_out;
    std::vector<int> rx_id_out;
    for (ConstGenParticlePtr pp: lx.first->particles_in()) lx_id_out.push_back(pp->pid());
    for (ConstGenParticlePtr pp: rx.first->particles_in()) rx_id_out.push_back(pp->pid());
    std::sort(lx_id_out.begin(), lx_id_out.end());
    std::sort(rx_id_out.begin(), rx_id_out.end());
    for (unsigned int i = 0; i < lx_id_out.size(); i++) if (lx_id_out[i] != rx_id_out[i]) return  (lx_id_out[i] < rx_id_out[i]);

    std::vector<double> lx_mom_in;
    std::vector<double> rx_mom_in;
    for (ConstGenParticlePtr pp: lx.first->particles_in()) lx_mom_in.push_back(pp->momentum().e());
    for (ConstGenParticlePtr pp: rx.first->particles_in()) rx_mom_in.push_back(pp->momentum().e());
    std::sort(lx_mom_in.begin(), lx_mom_in.end());
    std::sort(rx_mom_in.begin(), rx_mom_in.end());
    for (unsigned int i = 0; i < lx_mom_in.size(); i++) if (lx_mom_in[i] != rx_mom_in[i]) return  (lx_mom_in[i] < rx_mom_in[i]);

    std::vector<double> lx_mom_out;
    std::vector<double> rx_mom_out;
    for (ConstGenParticlePtr pp: lx.first->particles_in()) lx_mom_out.push_back(pp->momentum().e());
    for (ConstGenParticlePtr pp: rx.first->particles_in()) rx_mom_out.push_back(pp->momentum().e());
    std::sort(lx_mom_out.begin(), lx_mom_out.end());
    std::sort(rx_mom_out.begin(), rx_mom_out.end());
    for (unsigned int i = 0; i < lx_mom_out.size(); i++) if (lx_mom_out[i] != rx_mom_out[i]) return  (lx_mom_out[i] < rx_mom_out[i]);
    /* The code above is usefull mainly for debug*/

    return (lx.first < lx.first); /*This  is random. This should never happen*/
}
/** @brief Calculates the path to the top (beam) particles */
void calculate_longest_path_to_top(ConstGenVertexPtr v, std::map<ConstGenVertexPtr, int>& pathl)
{
    int p = 0;
    for (ConstGenParticlePtr pp: v->particles_in()) {
        ConstGenVertexPtr v2 = pp->production_vertex();
        if (v2 == v) continue; //LOOP! THIS SHOULD NEVER HAPPEN FOR A PROPER EVENT!
        if (!v2) p = std::max(p, 1);
        else
        {if (pathl.find(v2) == pathl.end())  calculate_longest_path_to_top(v2, pathl); p = std::max(p, pathl[v2]+1);}
    }
    pathl[v] = p;
    return;
}


}
