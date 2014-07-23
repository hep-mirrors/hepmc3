#ifndef RIVET_MATH_MATRIX3
#define RIVET_MATH_MATRIX3

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/MatrixN.hh"
#include "Rivet/Math/Vector3.hh"

namespace Rivet {


  /// @brief Specialisation of MatrixN to aid 3 dimensional rotations.
  class Matrix3 : public Matrix<3> {
  public:
    Matrix3() { }

    Matrix3(const Matrix<3>& m3) :  Matrix<3>::Matrix(m3) { }

    Matrix3(const Vector3& axis, const double angle) {
      const Vector3 normaxis = axis.unit();
      _matrix.loadRotation3(angle, normaxis._vec);
    }

    Matrix3(const Vector3& from, const Vector3& to) {
      setAsRotation(from, to);
    }

  public:
    static Matrix3 mkXRotation(const double angle) {
      return Matrix3(Vector3(1,0,0), angle);
    }

    static Matrix3 mkYRotation(const double angle) {
      return Matrix3(Vector3(0,1,0), angle);
    }

    static Matrix3 mkZRotation(const double angle) {
      return Matrix3(Vector3(0,0,1), angle);
    }

  public:
    Matrix3& setAsRotation(const Vector3& from, const Vector3& to) {
      const double theta = angle(from, to);
      if (Rivet::isZero(theta)) {
        _matrix.loadIdentity();
      } else {
        const Vector3 normaxis = cross(from, to).unit();
        _matrix.loadRotation3(theta, normaxis._vec);
      }
      return *this;
    }

  };


}

#endif
