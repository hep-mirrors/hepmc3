// -*- C++ -*-
#ifndef RIVET_ParisiTensor_HH
#define RIVET_ParisiTensor_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /**
     @brief Calculate the Parisi event shape tensor (or linear momentum tensor).
  
     The Parisi event shape C and D variables are derived from the eigenvalues of
     the linear momentum tensor
     \f[
     \theta^{\alpha \beta} =
     \frac{\sum_i \frac{p_i^\alpha p_i^\beta}{|\mathbf{p}_i|}}
          {\sum_i |\mathbf{p}_i|}
     \f]
     which is actually a linearized (and hence infra-red safe) version of the
     {@link Sphericity} tensor.

     Defining the three eigenvalues of \f$\theta\f$
     \f$ \lambda_1 \ge \lambda_2 \ge \lambda_3 \f$, with \f$ \lambda_1 + \lambda_2 + \lambda_3 = 1 \f$,
     the C and D parameters are defined as
     \f[
     C = 3(\lambda_1\lambda_2 + \lambda_1\lambda_3 + \lambda_2\lambda_3)
     \f]
     and
     \f[
     D = 27 \lambda_1\lambda_2\lambda_3
     \f]

     Internally, this Projection uses the Sphericity projection with the generalising
     \f$r\f$ parameter set to 1.
  */
  class ParisiTensor : public Projection {
  public:

    /// Constructor. The provided FinalState projection must live throughout the run.
    ParisiTensor(const FinalState& fsp)
    {
      setName("ParisiTensor");
      addProjection(fsp, "FS");
      addProjection(Sphericity(fsp, 1.0), "Sphericity");
      clear();
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new ParisiTensor(*this);
    }


  protected:

    /// Perform the projection on the Event.
    void project(const Event& e);

    /// Compare with other projections.
    int compare(const Projection& p) const;


  public:

    /// Clear the projection.
    void clear();


  public:

    /// @name Access the C and D params.
    ///@{
    double C() const { return _C; }
    double D() const { return _D; }
    ///@}

    /// @name Access the eigenvalues of \f$\theta\f$.
    ///@{
    double lambda1() const { return _lambda[0]; }
    double lambda2() const { return _lambda[1]; }
    double lambda3() const { return _lambda[2]; }
    ///@}

     
  private:
 
    /// The Parisi event shape variables.
    double _C, _D;

    /// Eigenvalues.
    double _lambda[3];

  };


}


#endif
