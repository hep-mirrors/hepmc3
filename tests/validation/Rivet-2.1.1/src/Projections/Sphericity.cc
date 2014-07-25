// -*- C++ -*-
#include "Rivet/Projections/Sphericity.hh"

namespace Rivet {


  Sphericity::Sphericity(const FinalState& fsp, double rparam)
    : _regparam(rparam)
  {
    setName("Sphericity");
    addProjection(fsp, "FS");
    clear();
  }


  void Sphericity::clear() {
    _lambdas = vector<double>(3, 0);
    _sphAxes = vector<Vector3>(3, Vector3());
  }


  int Sphericity::compare(const Projection& p) const {
    PCmp fscmp = mkNamedPCmp(p, "FS");
    if (fscmp != EQUIVALENT) return fscmp;
    const Sphericity& other = dynamic_cast<const Sphericity&>(p);
    if (fuzzyEquals(_regparam, other._regparam)) return 0;
    return cmp(_regparam, other._regparam);
  }


  void Sphericity::project(const Event& e) {
    const Particles prts = applyProjection<FinalState>(e, "FS").particles();
    calc(prts);
  }


  void Sphericity::calc(const FinalState& fs) {
    calc(fs.particles());
  }

  void Sphericity::calc(const vector<Particle>& fsparticles) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsparticles.size());
    foreach (const Particle& p, fsparticles) {
      threeMomenta.push_back( p.momentum().vector3() );
    }
    _calcSphericity(threeMomenta);
  }

  void Sphericity::calc(const vector<FourMomentum>& fsmomenta) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsmomenta.size());
    foreach (const FourMomentum& v, fsmomenta) {
      threeMomenta.push_back(v.vector3());
    }
    _calcSphericity(threeMomenta);
  }

  void Sphericity::calc(const vector<Vector3>& fsmomenta) {
    _calcSphericity(fsmomenta);
  }


  // Actually do the calculation
  void Sphericity::_calcSphericity(const vector<Vector3>& fsmomenta) {
    MSG_DEBUG("Calculating sphericity with r = " << _regparam);

    // Return (with "safe nonsense" sphericity params) if there are no final state particles.
    if (fsmomenta.empty()) {
      MSG_DEBUG("No particles in final state...");
      clear();
      return;
    }

    // Iterate over all the final state particles.
    Matrix3 mMom;
    double totalMomentum = 0.0;
    MSG_DEBUG("Number of particles = " << fsmomenta.size());
    foreach (const Vector3& p3, fsmomenta) {
      // Build the (regulated) normalising factor.
      totalMomentum += pow(p3.mod(), _regparam);

      // Build (regulated) quadratic momentum components.
      const double regfactor = pow(p3.mod(), _regparam-2);
      if (!fuzzyEquals(regfactor, 1.0)) {
        MSG_TRACE("Regfactor (r=" << _regparam << ") = " << regfactor);
      }

      Matrix3 mMomPart;
      for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          mMomPart.set(i,j, p3[i]*p3[j]);
        }
      }
      mMom += regfactor * mMomPart;
    }

    // Normalise to total (regulated) momentum.
    mMom /= totalMomentum;
    MSG_DEBUG("Momentum tensor = " << "\n" << mMom);

    // Check that the matrix is symmetric.
    const bool isSymm = mMom.isSymm();
    if (!isSymm) {
      MSG_ERROR("Error: momentum tensor not symmetric (r=" << _regparam << ")");
      MSG_ERROR("[0,1] vs. [1,0]: " << mMom.get(0,1) << ", " << mMom.get(1,0));
      MSG_ERROR("[0,2] vs. [2,0]: " << mMom.get(0,2) << ", " << mMom.get(2,0));
      MSG_ERROR("[1,2] vs. [2,1]: " << mMom.get(1,2) << ", " << mMom.get(2,1));
    }
    // If not symmetric, something's wrong (we made sure the error msg appeared first).
    assert(isSymm);

    // Diagonalize momentum matrix.
    const EigenSystem<3> eigen3 = diagonalize(mMom);
    MSG_DEBUG("Diag momentum tensor = " << "\n" << eigen3.getDiagMatrix());

    // Reset and set eigenvalue/vector parameters.
    _lambdas.clear();
    _sphAxes.clear();
    const EigenSystem<3>::EigenPairs epairs = eigen3.getEigenPairs();
    assert(epairs.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
      _lambdas.push_back(epairs[i].first);
      _sphAxes.push_back(Vector3(epairs[i].second));
    }

    // Debug output.
    MSG_DEBUG("Lambdas = ("
             << lambda1() << ", " << lambda2() << ", " << lambda3() << ")");
    MSG_DEBUG("Sum of lambdas = " << lambda1() + lambda2() + lambda3());
    MSG_DEBUG("Vectors = "
             << sphericityAxis() << ", "
             << sphericityMajorAxis() << ", "
             << sphericityMinorAxis() << ")");
  }

}
