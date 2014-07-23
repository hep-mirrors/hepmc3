#ifndef MT2_BISECT_H
#define MT2_BISECT_H

/***********************************************************************/
/*                                                                     */
/*              Finding mt2 by Bisection                               */
/*                                                                     */
/*              Authors: Hsin-Chia Cheng, Zhenyu Han                   */ 
/*              Dec 11, 2008, v1.01a                                   */
/*                                                                     */
/*              see arXiv:0810.5178                                    */  
/*                                                                     */  
/*              Wrapped for Rivet by D. Grellscheid                    */
/*              Apr 13, 2011                                           */
/*                                                                     */  
/***********************************************************************/

namespace Rivet {

namespace mt2_bisect
{

/*The user can change the desired precision below, the larger one of the following two definitions is used. Relative precision less than 0.00001 is not guaranteed to be achievable--use with caution*/ 

  const double RELATIVE_PRECISION = 0.00001; 
  //defined as precision = RELATIVE_PRECISION * scale, where scale = max{Ea, Eb}

  const double ABSOLUTE_PRECISION = 0.0;     
  //absolute precision for mt2, unused by default


//Reserved for expert
  const double MIN_MASS = 0.1; //if ma<MINMASS and mb<MINMASS, use massless code
  const double ZERO_MASS = 0.000; //give massless particles a small mass
  const double SCANSTEP = 0.1;

class mt2
{  
   public:

      mt2();
      void   mt2_bisect();
      void   mt2_massless();
      void   set_momenta(double *pa0, double *pb0, double* pmiss0);
      void   set_mn(double mn);
      double get_mt2();
  // void   print();
      int    nevt;
   private:  

      bool   solved;
      bool   momenta_set;
      double mt2_b;

      int    nsols(double Dsq);
      int    nsols_massless(double Dsq);
      inline int    signchange_n( long double t1, long double t2, long double t3, long double t4, long double t5);
      inline int    signchange_p( long double t1, long double t2, long double t3, long double t4, long double t5);
      int scan_high(double &Deltasq_high);
      int find_high(double &Deltasq_high);
      //data members
      double pax, pay, ma, Ea;
      double pmissx, pmissy;
      double pbx, pby, mb, Eb;
      double mn, mn_unscale;
     
      //auxiliary definitions
      double masq, Easq;
      double mbsq, Ebsq;
      double pmissxsq, pmissysq;
      double mnsq;

      //auxiliary coefficients
      double a1, b1, c1, a2, b2, c2, d1, e1, f1, d2, e2, f2;
      double d11, e11, f12, f10, d21, d20, e21, e20, f22, f21, f20;

      double scale;
      double precision;
};

}//end namespace mt2_bisect

}//end namespace Rivet

#endif
