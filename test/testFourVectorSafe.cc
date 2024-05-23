// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2024 The HepMC collaboration (see AUTHORS for details)
//
#include <iostream>
#include <fstream>
#include <vector>
#include "HepMC3/FourVector.h"
#include "HepMC3/PrintStreams.h"
#undef NDEBUG
#include <cassert>
#include <cfenv>
using namespace HepMC3;
int main()
{
    feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
    std::vector<FourVector>  vectors_to_test{
        FourVector(0.0, 0.0, 0.0, 0.0),
        FourVector(1.0, 2.0, 0.0, 0.0),
        FourVector(0.0, 0.0, 0.0, 1.0),
        FourVector(0.0, 0.0, 0.0,-1.0),
        FourVector(0.0, 0.0, 1.0, 0.0),
        FourVector(0.0, 0.0,-1.0, 0.0),
        FourVector(1.0, 2.0, 3.0, 4.0),
        FourVector(1.0, 2.0, 3.0, -4.0),
        FourVector(1.0, 2.0, -3.0, 4.0),
        FourVector(1.0, 2.0, -3.0, -4.0),
        FourVector(1.0, 2.0, -3.0, 40.0),
        FourVector(1.0, 2.0, -3.0, -40.0)
    };
    std::vector<double>   correct_eta{
        0.0,
        0.0,
        0.0,
        0.0,
        std::numeric_limits<double>::max(),
       -std::numeric_limits<double>::max(),
       std::atanh(3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0)),
       std::atanh(3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0)),
       std::atanh(-3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0)),
       std::atanh(-3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0)),
       std::atanh(-3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0)),
       std::atanh(-3.0/std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0))
    };

    std::vector<double>   correct_rap{
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        std::atanh(3.0/std::abs(4.0)),
        std::atanh(3.0/std::abs(4.0)),
        std::atanh(-3.0/std::abs(4.0)),
        std::atanh(-3.0/std::abs(-4.0)),
        std::atanh(-3.0/std::abs(40.0)),
        std::atanh(-3.0/std::abs(-40.0))

    };
    cout.setf(ios_base::scientific);
    cout.precision(12);
    cout.width(18);
    cout.setf(std::ios_base::showpos);
    for (size_t i = 0; i < vectors_to_test.size(); i++) {
        std::cout << "         eta() = " << vectors_to_test.at(i).safe_eta() << "         rap()=" << vectors_to_test.at(i).safe_rap() << " for " << vectors_to_test.at(i) << std::endl;
        std::cout << " Correct eta() = " << correct_eta.at(i)         << " Correct rap()=" << correct_rap.at(i) << std::endl << std::endl;
    }
    for (size_t i=0; i<vectors_to_test.size(); i++)
    {
        std::cout << "Testing " << vectors_to_test.at(i) << std::endl;
        assert(std::abs(vectors_to_test.at(i).safe_eta() - correct_eta.at(i)) < std::numeric_limits<double>::epsilon() || vectors_to_test.at(i).safe_eta() == correct_eta.at(i));
        assert(std::abs(vectors_to_test.at(i).safe_rap() - correct_rap.at(i)) < std::numeric_limits<double>::epsilon() || vectors_to_test.at(i).safe_rap() == correct_rap.at(i));

    }
    return 0;
}
