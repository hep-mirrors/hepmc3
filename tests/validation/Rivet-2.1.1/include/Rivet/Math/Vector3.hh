#ifndef RIVET_MATH_VECTOR3
#define RIVET_MATH_VECTOR3

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/VectorN.hh"

namespace Rivet {


  class Vector3;
  typedef Vector3 ThreeVector;
  class Matrix3;

  Vector3 multiply(const double, const Vector3&);
  Vector3 multiply(const Vector3&, const double);
  Vector3 add(const Vector3&, const Vector3&);
  Vector3 operator*(const double, const Vector3&);
  Vector3 operator*(const Vector3&, const double);
  Vector3 operator/(const Vector3&, const double);
  Vector3 operator+(const Vector3&, const Vector3&);
  Vector3 operator-(const Vector3&, const Vector3&);


  /// @brief Three-dimensional specialisation of Vector.
  class Vector3 : public Vector<3> {

    friend class Matrix3;
    friend Vector3 multiply(const double, const Vector3&);
    friend Vector3 multiply(const Vector3&, const double);
    friend Vector3 add(const Vector3&, const Vector3&);
    friend Vector3 subtract(const Vector3&, const Vector3&);

  public:
    Vector3() : Vector<3>() { }

    template<typename V3>
    Vector3(const V3& other) {
      this->setX(other.x());
      this->setY(other.y());
      this->setZ(other.z());
    }

    Vector3(const Vector<3>& other) {
      this->setX(other.get(0));
      this->setY(other.get(1));
      this->setZ(other.get(2));
    }

    Vector3(double x, double y, double z) {
      this->setX(x);
      this->setY(y);
      this->setZ(z);
    }

    ~Vector3() { }

  public:
    static Vector3 mkX() { return Vector3(1,0,0); }
    static Vector3 mkY() { return Vector3(0,1,0); }
    static Vector3 mkZ() { return Vector3(0,0,1); }

  public:
    double x() const { return get(0); }
    double y() const { return get(1); }
    double z() const { return get(2); }
    Vector3& setX(double x) { set(0, x); return *this; }
    Vector3& setY(double y) { set(1, y); return *this; }
    Vector3& setZ(double z) { set(2, z); return *this; }

    double dot(const Vector3& v) const {
      return _vec.dot(v._vec);
    }

    Vector3 cross(const Vector3& v) const {
      Vector3 result;
      result._vec = _vec.cross(v._vec);
      return result;
    }

    double angle(const Vector3& v) const {
      const double localDotOther = unit().dot(v.unit());
      if (fuzzyEquals(localDotOther, 1.0)) return 0.0;
      else if (fuzzyEquals(localDotOther, -1.0)) return M_PI;
      return acos(localDotOther);
    }

    Vector3 unit() const {
      /// @todo What to do in this situation?
      if (isZero()) return *this;
      else return *this * 1.0/this->mod();
    }

    double polarRadius2() const {
      return x()*x() + y()*y();
    }

    /// Synonym for polarRadius2
    double perp2() const {
      return polarRadius2();
    }

    /// Synonym for polarRadius2
    double rho2() const {
      return polarRadius2();
    }

    double polarRadius() const {
      return sqrt(polarRadius2());
    }

    /// Synonym for polarRadius
    double perp() const {
      return polarRadius();
    }

    /// Synonym for polarRadius
    double rho() const {
      return polarRadius();
    }

    /// Angle subtended by the vector's projection in x-y and the x-axis.
    double azimuthalAngle(const PhiMapping mapping = ZERO_2PI) const {
      // If this is a null vector, return zero rather than let atan2 set an error state
      if (Rivet::isZero(mod2())) return 0.0;

      // Calculate the arctan and return in the requested range
      const double value = atan2( y(), x() );
      return mapAngle(value, mapping);
    }

    /// Synonym for azimuthalAngle.
    double phi(const PhiMapping mapping = ZERO_2PI) const {
      return azimuthalAngle(mapping);
    }

    /// Angle subtended by the vector and the z-axis.
    double polarAngle() const {
      // Get number beween [0,PI]
      const double polarangle = atan2(polarRadius(), z());
      return mapAngle0ToPi(polarangle);
    }

    /// Synonym for polarAngle
    double theta() const {
      return polarAngle();
    }

    /// Purely geometric approximation to rapidity; exact for massless particles
    /// and in the central region.
    double pseudorapidity() const {
      return -std::log(tan( 0.5 * polarAngle() ));
    }

    /// Synonym for pseudorapidity.
    double eta() const {
      return pseudorapidity();
    }

  public:
    Vector3& operator*=(const double a) {
      _vec = multiply(a, *this)._vec;
      return *this;
    }

    Vector3& operator/=(const double a) {
      _vec = multiply(1.0/a, *this)._vec;
      return *this;
    }

    Vector3& operator+=(const Vector3& v) {
      _vec = add(*this, v)._vec;
      return *this;
    }

    Vector3& operator-=(const Vector3& v) {
      _vec = subtract(*this, v)._vec;
      return *this;
    }

    Vector3 operator-() const {
      Vector3 rtn;
      rtn._vec = -_vec;
      return rtn;
    }

  };



  inline double dot(const Vector3& a, const Vector3& b) {
    return a.dot(b);
  }

  inline Vector3 cross(const Vector3& a, const Vector3& b) {
    return a.cross(b);
  }

  inline Vector3 multiply(const double a, const Vector3& v) {
    Vector3 result;
    result._vec = a * v._vec;
    return result;
  }

  inline Vector3 multiply(const Vector3& v, const double a) {
    return multiply(a, v);
  }

  inline Vector3 operator*(const double a, const Vector3& v) {
    return multiply(a, v);
  }

  inline Vector3 operator*(const Vector3& v, const double a) {
    return multiply(a, v);
  }

  inline Vector3 operator/(const Vector3& v, const double a) {
    return multiply(1.0/a, v);
  }

  inline Vector3 add(const Vector3& a, const Vector3& b) {
    Vector3 result;
    result._vec = a._vec + b._vec;
    return result;
  }

  inline Vector3 subtract(const Vector3& a, const Vector3& b) {
    Vector3 result;
    result._vec = a._vec - b._vec;
    return result;
  }

  inline Vector3 operator+(const Vector3& a, const Vector3& b) {
    return add(a, b);
  }

  inline Vector3 operator-(const Vector3& a, const Vector3& b) {
    return subtract(a, b);
  }

  // More physicsy coordinates etc.

  /// Angle (in radians) between two 3-vectors.
  inline double angle(const Vector3& a, const Vector3& b) {
    return a.angle(b);
  }

  /// Calculate transverse length sq. \f$ \rho^2 \f$ of a 3-vector.
  inline double polarRadius2(const Vector3& v) {
    return v.polarRadius2();
  }
  /// Synonym for polarRadius2.
  inline double perp2(const Vector3& v) {
    return v.perp2();
  }
  /// Synonym for polarRadius2.
  inline double rho2(const Vector3& v) {
    return v.rho2();
  }

  /// Calculate transverse length \f$ \rho \f$ of a 3-vector.
  inline double polarRadius(const Vector3& v) {
    return v.polarRadius();
  }
  /// Synonym for polarRadius.
  inline double perp(const Vector3& v) {
    return v.perp();
  }
  /// Synonym for polarRadius.
  inline double rho(const Vector3& v) {
    return v.rho();
  }


  /// @brief Calculate azimuthal angle of a 3-vector.
  /// Returns a number in (-pi, pi] or in [0, 2pi) according to the mapping scheme selected
  inline double azimuthalAngle(const Vector3& v, const PhiMapping mapping = ZERO_2PI) {
    return v.azimuthalAngle(mapping);
  }
  /// Synonym for azimuthalAngle.
  inline double phi(const Vector3& v, const PhiMapping mapping = ZERO_2PI) {
    return v.phi(mapping);
  }

  /// Calculate polar angle of a 3-vector.
  inline double polarAngle(const Vector3& v) {
    return v.polarAngle();
  }
  /// Synonym for polarAngle.
  inline double theta(const Vector3& v) {
    return v.theta();
  }

  /// Calculate pseudorapidity of a 3-vector.
  inline double pseudorapidity(const Vector3& v) {
    return v.pseudorapidity();
  }
  /// Synonym for pseudorapidity.
  inline double eta(const Vector3& v) {
    return v.eta();
  }


  /////////////////////////////////////////////////////

  /// @name \f$ |\Delta eta| \f$ calculations from 3-vectors
  //@{

  /// Calculate the difference in pseudorapidity between two spatial vectors.
  inline double deltaEta(const Vector3& a, const Vector3& b) {
    return deltaEta(a.pseudorapidity(), b.pseudorapidity());
  }

  /// Calculate the difference in pseudorapidity between two spatial vectors.
  inline double deltaEta(const Vector3& v, double eta2) {
    return deltaEta(v.pseudorapidity(), eta2);
  }

  /// Calculate the difference in pseudorapidity between two spatial vectors.
  inline double deltaEta(double eta1, const Vector3& v) {
    return deltaEta(eta1, v.pseudorapidity());
  }

  //@}


  /// @name \f$ \Delta phi \f$ calculations from 3-vectors
  //@{

  /// Calculate the difference in azimuthal angle between two spatial vectors.
  inline double deltaPhi(const Vector3& a, const Vector3& b) {
    return deltaPhi(a.azimuthalAngle(), b.azimuthalAngle());
  }

  /// Calculate the difference in azimuthal angle between two spatial vectors.
  inline double deltaPhi(const Vector3& v, double phi2) {
    return deltaPhi(v.azimuthalAngle(), phi2);
  }

  /// Calculate the difference in azimuthal angle between two spatial vectors.
  inline double deltaPhi(double phi1, const Vector3& v) {
    return deltaPhi(phi1, v.azimuthalAngle());
  }

  //@}


  /// @name \f$ \Delta R \f$ calculations from 3-vectors
  //@{

  /// Calculate the 2D rapidity-azimuthal ("eta-phi") distance between two spatial vectors.
  inline double deltaR(const Vector3& a, const Vector3& b) {
    return deltaR(a.pseudorapidity(), a.azimuthalAngle(),
                  b.pseudorapidity(), b.azimuthalAngle());
  }

  /// Calculate the 2D rapidity-azimuthal ("eta-phi") distance between two spatial vectors.
  inline double deltaR(const Vector3& v, double eta2, double phi2) {
    return deltaR(v.pseudorapidity(), v.azimuthalAngle(), eta2, phi2);
  }

  /// Calculate the 2D rapidity-azimuthal ("eta-phi") distance between two spatial vectors.
  inline double deltaR(double eta1, double phi1, const Vector3& v) {
    return deltaR(eta1, phi1, v.pseudorapidity(), v.azimuthalAngle());
  }

  //@}

}

#endif
