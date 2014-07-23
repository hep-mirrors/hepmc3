// -*- C++ -*-
#ifndef RIVET_DISKinematics_HH
#define RIVET_DISKinematics_HH

#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/DISLepton.hh"
#include "Rivet/Projections/Beam.hh"

namespace Rivet {


  /// @brief Get the DIS kinematic variables and relevant boosts for an event.
  class DISKinematics : public Projection {

  public:

    /// The default constructor.
    DISKinematics()
      : _theQ2(-1.0), _theW2(-1.0), _theX(-1.0), _theY(-1.0), _theS(-1.0)
    {
      setName("DISKinematics");
      //addPdgIdPair(ANY, hadid);
      addProjection(Beam(), "Beam");
      addProjection(DISLepton(), "Lepton");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new DISKinematics(*this);
    }


  protected:

    /// Perform the projection operation on the supplied event.
    virtual void project(const Event& e);

    /// Compare with other projections.
    virtual int compare(const Projection& p) const;


  public:

    /// The \f$Q^2\f$.
    double Q2() const { return _theQ2; }

    /// The \f$W^2\f$.
    double W2() const { return _theW2; }

    /// The Bjorken \f$x\f$.
    double x() const { return _theX; }

    /// The Inelasticity \f$y\f$
    double y() const { return _theY; }

    /// The centre of mass energy \f$s\f$
    double s() const { return _theS; }



    /// The LorentzRotation needed to boost a particle to the hadronic CM frame.
    const LorentzTransform& boostHCM() const {
      return _hcm;
    }

    /// The LorentzRotation needed to boost a particle to the hadronic Breit frame.
    const LorentzTransform& boostBreit() const {
      return _breit;
    }

    /// The incoming Hadron beam particle
    const Particle& beamHadron() const {
      return _inHadron;
    }

  private:

    /// The \f$Q^2\f$.
    double _theQ2;

    /// The \f$W^2\f$.
    double _theW2;

    /// The Bjorken \f$x\f$.
    double _theX;

    /// The Inelasticity \f$y\f$
    double _theY;

    /// The centre of mass energy \f$s\f$
    double _theS;

    Particle _inHadron;

    /// The LorentzRotation needed to boost a particle to the hadronic CM frame.
    LorentzTransform _hcm;

    /// The LorentzRotation needed to boost a particle to the hadronic Breit frame.
    LorentzTransform _breit;
  };


}

#endif
