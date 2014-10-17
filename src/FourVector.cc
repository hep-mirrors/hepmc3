#include "HepMC/FourVector.h"
#include <boost/static_assert.hpp>

namespace HepMC {

bool FourVector::AlmostEqual2sComplement(double A, double B, unsigned int maxUlps) {

    // @todo Currently only works on 64-bit machine 
    BOOST_STATIC_ASSERT( sizeof(double)==8 && sizeof(long long)==8 );
    static const long long MASK = 0x8000000000000000;

    // Not equal if sign is not equal
    bool isNegativeA = (A < 0);
    bool isNegativeB = (B < 0);
    if( isNegativeA != isNegativeB ) return false;

    long long *aInt = reinterpret_cast<long long*>(&A);
    // Make aInt lexicographically ordered as a twos-complement int
    if( *aInt < 0 )
        *aInt = MASK - *aInt;

    // Make bInt lexicographically ordered as a twos-complement int
    long long *bInt = reinterpret_cast<long long*>(&B);
    if( *bInt < 0 )
        *bInt = MASK - *bInt;

    long long intDiff = aInt - bInt;
    if( intDiff < 0 ) intDiff= intDiff*-1;

    if( intDiff <= maxUlps ) return true;

    return false;
}
}
