// -*- C++ -*-
#ifndef RIVET_Hemispheres_HH
#define RIVET_Hemispheres_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/AxesDefinition.hh"
#include "Rivet/Event.hh"


namespace Rivet {

  /// @brief Calculate the hemisphere masses and broadenings.
  ///
  /// Calculate the hemisphere masses and broadenings, with event hemispheres
  /// defined by the plane normal to the thrust vector, \f$ \vec{n}_\mathrm{T} \f$.
  ///
  /// The "high" hemisphere mass,
  /// \f$ M^2_\mathrm{high} / E^2_\mathrm{vis} \f$, is defined as
  /// \f[
  /// \frac{M^2_\mathrm{high}}{E^2_\mathrm{vis}} =
  /// \frac{1}{E^2_\mathrm{vis}} \max
  /// \left(
  /// \left| \sum_{\vec{p}_k \cdot \vec{n}_\mathrm{T} > 0} p_k \right|^2 ,
  /// \left| \sum_{\vec{p}_k \cdot \vec{n}_\mathrm{T} < 0} p_k \right|^2
  /// \right)
  /// \f]
  /// and the corresponding "low" hemisphere mass,
  /// \f$ M^2_\mathrm{low} / E^2_\mathrm{vis} \f$,
  /// is the sum of momentum vectors in the opposite hemisphere, i.e.
  /// \f$ \max \rightarrow \min \f$ in the formula above.
  ///
  /// Finally, we define a hemisphere mass difference:
  /// \f[
  /// \frac{M^2_\mathrm{diff} }{ E^2_\mathrm{vis}} =
  /// \frac{ M^2_\mathrm{high} - M^2_\mathrm{low} }{ E^2_\mathrm{vis}} .
  /// \f]
  ///
  /// Similarly to the masses, we also define hemisphere broadenings, using the
  /// momenta transverse to the thrust axis:
  /// \f[
  /// B_\pm =
  /// \frac{
  ///   \sum{\pm \vec{p}_i \cdot \vec{n}_\mathrm{T} > 0}
  ///   |\vec{p}_i \times \vec{n}_\mathrm{T} |
  /// }{
  ///   2 \sum_i | \vec{p}_i |
  /// }
  /// \f]
  /// and then a set of the broadening maximum, minimum, sum and difference as follows:
  /// \f[ B_\mathrm{max}  = \max(B_+, B_-) \f]
  /// \f[ B_\mathrm{min}  = \min(B_+, B_-) \f]
  /// \f[ B_\mathrm{sum}  = B_+ + B_- \f]
  /// \f[ B_\mathrm{diff} = |B_+ - B_-| \f]
  ///
  /// Internally, this projection uses a Thrust or Sphericity projection to
  /// determine the hemisphere orientation.
  class Hemispheres : public Projection {
  public:

    /// Constructor.
    Hemispheres(const AxesDefinition& ax) {
      setName("Hemispheres");
      addProjection(ax, "Axes");
      clear();
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new Hemispheres(*this);
    }

    // Reset the projection
    void clear() {
      _E2vis = -1;
      _M2high = -1;
      _M2low = -1;
      _Bmax = -1;
      _Bmin = -1;
      _highMassEqMaxBroad = true;
    }


  protected:

    /// Perform the projection on the Event.
    void project(const Event& e);

    /// Compare with other projections.
    int compare(const Projection& p) const {
      return mkNamedPCmp(p, "Axes");
    }


  public:

    /// @name Hemisphere masses (scaled by \f$ 1 / E^2_\mathrm{vis} \f$).
    //@{

    double E2vis() const { return _E2vis; }
    double Evis() const { return sqrt(_E2vis); }

    double M2high() const { return _M2high; }
    double Mhigh() const { return sqrt(M2high()); }

    double M2low() const { return _M2low; }
    double Mlow() const { return sqrt(M2low()); }

    double M2diff() const { return _M2high -_M2low; }
    double Mdiff() const { return sqrt(M2diff()); }

    double scaledM2high() const {
      if (isZero(_M2high)) return 0.0;
      if (!isZero(_E2vis)) return _M2high/_E2vis;
      else return std::numeric_limits<double>::max();
    }
    double scaledMhigh() const { return sqrt(scaledM2high()); }

    double scaledM2low() const {
      if (isZero(_M2low)) return 0.0;
      if (!isZero(_E2vis)) return _M2low/_E2vis;
      else return std::numeric_limits<double>::max();
    }
    double scaledMlow() const { return sqrt(scaledM2low()); }

    double scaledM2diff() const {
      if (M2diff() == 0.0) return 0.0;
      if (_E2vis != 0.0) return M2diff()/_E2vis;
      else return std::numeric_limits<double>::max();
    }
    double scaledMdiff() const { return sqrt(scaledM2diff()); }
    //@}


    /// @name Hemisphere broadenings.
    //@{
    double Bmax() const { return _Bmax; }
    double Bmin() const { return _Bmin; }
    double Bsum() const { return _Bmax + _Bmin; }
    double Bdiff() const { return fabs(_Bmax - _Bmin); } // <- fabs(), just in case...
    //@}


    /// Is the hemisphere with the max mass the same as the one with the max broadening?
    bool massMatchesBroadening() {
      return _highMassEqMaxBroad;
    }


  private:

    /// Visible energy-squared, \f$ E^2_\mathrm{vis} \f$.
    double _E2vis;

    /// Hemisphere mass variables.
    double _M2high, _M2low;

    /// Hemisphere broadening variables.
    double _Bmax, _Bmin;

    /// Is the hemisphere with the max mass the same as the one with the max broadening?
    bool _highMassEqMaxBroad;

  };


}

#endif
