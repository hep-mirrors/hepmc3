#include "Rivet/Tools/RivetMT2.hh"
#include "Rivet/Math/Vector4.hh"
#include "mt2_bisect.hh"


namespace Rivet {
  namespace mT2 {
      
    double mT2(const FourMomentum & a, 
	       const FourMomentum & b, 
	       const FourMomentum & pTmiss, 
	       double invisiblesMass) 
    {
      mt2_bisect::mt2 mt2_event;
      
      double unused = -999.999;
      
      double pa[3]    = { 0.0, a.x(), a.y() };
      double pb[3]    = { 0.0, b.x(), b.y() };
      double pmiss[3] = { unused, pTmiss.x(), pTmiss.y() };
      
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( invisiblesMass );
      
      return mt2_event.get_mt2();
    }

  }
}
