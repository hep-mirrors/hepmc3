// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenRunInfo.cc
 *  @brief Implementation of \b class GenRunInfo
 *
 */
#include "HepMC/GenRunInfo.h"
#include <sstream>

namespace HepMC {

void GenRunInfo::set_weight_names(const vector<string> & names) {
    m_weight_indices.clear();
    m_weight_names = names;
    for ( int i = 0, N = names.size(); i < N; ++i ) {
	string name = names[i];
	if ( name.empty() ) {
	    std::ostringstream oss;
	    oss << i;
	    name = oss.str();
	}
	if ( has_weight(name) )
	    throw std::logic_error("GenRunInfo::set_weight_names: "
				   "Duplicate weight name '" + name + 
				   "' found.");
	m_weight_indices[name] = i;
    }
}

} // namespace HepMC
