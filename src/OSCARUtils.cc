// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2024 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file OSCARUtils.cc
/// @brief Implementation of \b function tokenize_string
///
#include <string>
#include <vector>

namespace HepMC3 {

std::vector<std::string> tokenize_string(const std::string& str, const std::string& delimiters )
{
    std::vector<std::string> tokens;
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.emplace_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
}

} // namespace HepMC3
