// -*- C++ -*-
#ifndef RIVET_VetoedFinalState_HH
#define RIVET_VetoedFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief FS modifier to exclude classes of particles from the final state.
  class VetoedFinalState : public FinalState {

  public:

    /// Typedef for a pair of back-to-back cuts.
    typedef pair<double, double> BinaryCut;

    /// Typedef for a vetoing entry.
    typedef map<long, BinaryCut> VetoDetails;

    /// Typedef for a veto on a composite particle mass.
    typedef multimap<int, BinaryCut>  CompositeVeto;


    /// @name Constructors
    //@{
    /// Default constructor.
    VetoedFinalState() {
      setName("VetoedFinalState");
      addProjection(FinalState(), "FS");
    }

    /// Constructor with specific FinalState.
    VetoedFinalState(const FinalState& fsp)
    {
      setName("VetoedFinalState");
      addProjection(fsp, "FS");
    }

    /// You can add a map of ID plus a pair containing \f$ p_{Tmin} \f$ and
    /// \f$ p_{Tmax} \f$ - these define the range of particles to be vetoed.
    VetoedFinalState(const VetoDetails& vetocodes)
      : _vetoCodes(vetocodes)
    {
      setName("VetoedFinalState");
      addProjection(FinalState(), "FS");
    }

    /// You can add a map of ID plus a pair containing \f$ p_{Tmin} \f$ and
    /// \f$ p_{Tmax} \f$ - these define the range of particles to be vetoed.
    /// This version also supplies a specifi FinalState to be used.
    VetoedFinalState(const FinalState& fsp, const VetoDetails& vetocodes)
      : _vetoCodes(vetocodes)
    {
      setName("VetoedFinalState");
      addProjection(fsp, "FS");
    }


    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new VetoedFinalState(*this);
    }
    //@}


  public:

    /// Get the list of particle IDs and \f$ p_T \f$ ranges to veto.
    const VetoDetails& vetoDetails() const {
      return _vetoCodes;
    }

    /// Add a particle ID and \f$ p_T \f$ range to veto. Particles with \f$ p_T \f$
    /// IN the given range will be rejected.
    VetoedFinalState& addVetoDetail(const long id, const double ptmin, const double ptmax) {
      BinaryCut ptrange(ptmin, ptmax);
      _vetoCodes.insert(make_pair(id, ptrange));
      return *this;
    }

    /// Add a particle/antiparticle pair to veto in a given \f$ p_T \f$ range. Given a single ID, both
    /// the particle and its conjugate antiparticle will be rejected if their \f$ p_T \f$ is IN the given range.
    VetoedFinalState& addVetoPairDetail(const long id, const double ptmin, const double ptmax) {
      addVetoDetail(id,  ptmin, ptmax);
      addVetoDetail(-id, ptmin, ptmax);
      return *this;
    }

    /// Add a particle/antiparticle pair to veto. Given a single ID, both the particle and its corresponding
    /// antiparticle (for all \f$ p_T \f$ values) will be vetoed.
    VetoedFinalState& addVetoPairId(const long id) {
      addVetoId(id);
      addVetoId(-id);
      return *this;
    }

    /// Add a particle ID to veto (all \f$ p_T \f$ range will be vetoed).
    VetoedFinalState& addVetoId(const long id) {
      BinaryCut ptrange(0.0, numeric_limits<double>::max());
      _vetoCodes.insert(make_pair(id, ptrange));
      return *this;
    }

    /// Veto all neutrinos (convenience method)
    VetoedFinalState& vetoNeutrinos() {
      addVetoPairId(PID::NU_E);
      addVetoPairId(PID::NU_MU);
      addVetoPairId(PID::NU_TAU);
      return *this;
    }

    /// Add a veto on composite masses within a given width.
    /// The composite mass is composed of nProducts decay products
    /// @ todo might we want to specify a range of pdg ids for the decay products?
    VetoedFinalState& addCompositeMassVeto(const double &mass, const double &width, int nProducts=2){
      double halfWidth = 0.5*width;
      BinaryCut massRange(mass - halfWidth, mass + halfWidth);
      _compositeVetoes.insert(make_pair(nProducts, massRange));
      _nCompositeDecays.insert(nProducts);
      return *this;
    }

    /// Veto the decay products of particle with pdg id
    /// @todo Need HepMC to sort themselves out and keep vector bosons from
    /// the hard vtx in the event record before this will work reliably for all pdg ids
    VetoedFinalState& addDecayProductsVeto(const long id){
      _parentVetoes.insert(id);
      return *this;
    }

    /// Set the list of particle IDs and \f$ p_T \f$ ranges to veto.
    VetoedFinalState& setVetoDetails(const VetoDetails& ids) {
      _vetoCodes = ids;
      return *this;
    }

    /// Clear the list of particle IDs and ranges to veto.
    VetoedFinalState& reset() {
      _vetoCodes.clear();
      return *this;
    }


    /// Veto particles from a supplied final state.
    VetoedFinalState& addVetoOnThisFinalState(const FinalState& fs) {
      stringstream st_name;
      st_name << "FS_" << _vetofsnames.size();
      string name = st_name.str();
      addProjection(fs, name);
      _vetofsnames.insert(name);
      return *this;
    }


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;


  private:

    /// The final-state particles.
    VetoDetails _vetoCodes;

    /// Composite particle masses to veto
    CompositeVeto _compositeVetoes;
    set<int> _nCompositeDecays;

    typedef set<long> ParentVetos;

    /// Set of decaying particle IDs to veto
    ParentVetos _parentVetoes;

    /// Set of finalstate to be vetoed
    set<string> _vetofsnames;

  };


}


#endif
