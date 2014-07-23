// -*- C++ -*-
#ifndef RIVET_MT2_HH
#define RIVET_MT2_HH

// Convenience wrapper for the m_T2 calculator of Cheng/Han
// (arXiv:0810.5178). Could be adapted for other backends in future

namespace Rivet {

  class FourMomentum;

  namespace mT2 {
    
    double mT2(const FourMomentum & a, 
	       const FourMomentum & b, 
	       const FourMomentum & pTmiss, 
	       double invisiblesMass);

  }
}


#endif
