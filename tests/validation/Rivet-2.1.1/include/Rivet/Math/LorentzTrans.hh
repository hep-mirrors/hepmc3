#ifndef RIVET_MATH_LORENTZTRANS
#define RIVET_MATH_LORENTZTRANS

#include <iostream>

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/MatrixN.hh"
#include "Rivet/Math/Matrix3.hh"
#include "Rivet/Math/Vector4.hh"

namespace Rivet {


  inline double lorentzGamma(const double beta) {
    return 1.0 / sqrt(1 - beta*beta);
  }


  /// @brief Object implementing Lorentz transform calculations and boosts.
  class LorentzTransform {
    friend string toString(const LorentzTransform& lt);

  public:
    LorentzTransform() {
      _boostMatrix = Matrix<4>::mkIdentity();
    }

    LorentzTransform(const Vector3& boost) {
      setBoost(boost);
    }

    LorentzTransform(const double betaX, const double betaY, const double betaZ) {
      setBoost(betaX, betaY, betaZ);
    }

    LorentzTransform& setBoost(const Vector3& boost) {
      assert(boost.mod2() < 1);
      const double beta = boost.mod();
      const double gamma = lorentzGamma(beta);
      _boostMatrix = Matrix<4>::mkIdentity();
      _boostMatrix.set(0, 0, gamma);
      _boostMatrix.set(1, 1, gamma);
      // Positive coeff since these are active boosts
      _boostMatrix.set(0, 1, +beta*gamma);
      _boostMatrix.set(1, 0, +beta*gamma);
      _boostMatrix = rotate(Vector3::mkX(), boost)._boostMatrix;
      return *this;
    }

  //   LorentzTransform& setBoost(const Vector3& boostdirection, const double beta) {
  //     const Vector3 boost = boostdirection.unit() * beta;
  //     return setBoost(boost);
  //   }

    LorentzTransform& setBoost(const double betaX, const double betaY, const double betaZ) {
      return setBoost(Vector3(betaX, betaY, betaZ));
    }

    Vector3 boost() const {
      FourMomentum boost(_boostMatrix.getColumn(0));
      //cout << "!!!" << boost << endl;
      if (boost.isZero()) return boost;
      assert(boost.E() > 0);
      const double beta = boost.p().mod() / boost.E();
      return boost.p().unit() * beta;
    }

    double beta() const {
      return boost().mod();
    }

    double gamma() const {
      return lorentzGamma(beta());
    }

    LorentzTransform rotate(const Vector3& from, const Vector3& to) const {
      return rotate(Matrix3(from, to));
    }

    LorentzTransform rotate(const Vector3& axis, const double angle) const {
      return rotate(Matrix3(axis, angle));
    }

    LorentzTransform rotate(const Matrix3& rot) const {
      LorentzTransform lt = *this;
      const Matrix4 rot4 = mkMatrix4(rot);
      const Matrix4 newlt = rot4 * _boostMatrix * rot4.inverse();
      lt._boostMatrix = newlt;
      return lt;
    }

    FourVector transform(const FourVector& v4) const {
      return multiply(_boostMatrix, v4);
    }

    LorentzTransform inverse() const {
      LorentzTransform rtn;
      rtn._boostMatrix = _boostMatrix.inverse();
      return rtn;
    }


    /// Combine LTs, treating @a this as the LH matrix.
    LorentzTransform combine(const LorentzTransform& lt) const {
      LorentzTransform rtn;
      rtn._boostMatrix = _boostMatrix * lt._boostMatrix;
      return rtn;
    }

    Matrix4 toMatrix() const {
      return _boostMatrix;
    }


    LorentzTransform operator*(const LorentzTransform& lt) const {
      return combine(lt);
    }

    LorentzTransform preMult(const Matrix3 & m3) {
      _boostMatrix = multiply(mkMatrix4(m3),_boostMatrix);
      return *this;
    }

    LorentzTransform postMult(const Matrix3 & m3) {
      _boostMatrix *= mkMatrix4(m3);
      return *this;
    }

  private:
    Matrix4 mkMatrix4(const Matrix3& m3) const {
      Matrix4 m4 = Matrix4::mkIdentity();
      for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          m4.set(i+1, j+1, m3.get(i, j));
        }
      }
      return m4;
    }


  private:
    Matrix4 _boostMatrix;

  };



  inline LorentzTransform inverse(const LorentzTransform& lt) {
    return lt.inverse();
  }

  inline LorentzTransform combine(const LorentzTransform& a, const LorentzTransform& b) {
    return a.combine(b);
  }

  inline FourVector transform(const LorentzTransform& lt, const FourVector& v4) {
      return lt.transform(v4);
  }


  //////////////////////////


  inline string toString(const LorentzTransform& lt) {
    return toString(lt._boostMatrix);
  }

  inline ostream& operator<<(std::ostream& out, const LorentzTransform& lt) {
    out << toString(lt);
    return out;
  }


}

#endif
