#ifndef RIVET_MATH_VECTORN
#define RIVET_MATH_VECTORN

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MathUtils.hh"

#include "Rivet/Math/eigen/vector.h"

namespace Rivet {


  template <size_t N>
  class Vector;
  template <size_t N>
  class Matrix;

  template <size_t N>
  Vector<N> multiply(const Matrix<N>& a, const Vector<N>& b);


  /// A minimal base class for \f$ N \f$-dimensional vectors.
  template <size_t N>
  class Vector {
    template <size_t M>
    friend Vector<M> multiply(const Matrix<M>& a, const Vector<M>& b);

  public:
    Vector() { _vec.loadZero(); }

    Vector(const Vector<N>& other)
      : _vec(other._vec) { }

    const double& get(const size_t index) const {
      if (index >= N) {
        throw std::runtime_error("Tried to access an invalid vector index.");
      } else {
        return _vec(index);
      }
    }

    /// Direct access to vector elements by index.
    const double& operator[](const size_t index) const {
      return get(index);
    }

    /// Direct access to vector elements by index.
    double& operator[](const size_t index) {
      return get(index);
    }

    /// Set indexed value
    Vector<N>& set(const size_t index, const double value) {
      if (index >= N) {
        throw std::runtime_error("Tried to access an invalid vector index.");
      } else {
        _vec[index] = value;
      }
      return *this;
    }

    /// Vector dimensionality
    size_t size() const {
      return N;
    }

    /// Check for nullness, allowing for numerical precision
    bool isZero(double tolerance=1E-5) const {
      for (size_t i=0; i < N; ++i) {
        if (! Rivet::isZero(_vec[i], tolerance) ) return false;
      }
      return true;
    }

    /// @brief Calculate the modulus-squared of a vector.
    /// \f$ \sum_{i=1}^N x_i^2 \f$.
    double mod2() const {
      double mod2 = 0.0;
      for (size_t i = 0; i < size(); ++i) {
        const double element = get(i);
        mod2 += element*element;
      }
      return mod2;
    }

    /// @brief Calculate the modulus of a vector.
    /// \f$ \sqrt{\sum_{i=1}^N x_i^2} \f$.
    double mod() const {
      const double norm = mod2();
      assert(norm >= 0);
      return sqrt(norm);
    }

    /// Invert the vector
    Vector<N> operator-() const {
      Vector<N> rtn;
      rtn._vec = -_vec;
      return rtn;
    }

    bool operator==(const Vector<N>& a) const {
      return _vec == a._vec;
    }

    bool operator!=(const Vector<N>& a) const {
      return _vec != a._vec;
    }

    bool operator<(const Vector<N>& a) const {
      return _vec < a._vec;
    }

    bool operator<=(const Vector<N>& a) const {
      return _vec <= a._vec;
    }

    bool operator>(const Vector<N>& a) const {
      return _vec > a._vec;
    }

    bool operator>=(const Vector<N>& a) const {
      return _vec >= a._vec;
    }


  protected:
    double& get(const size_t index) {
      if (index >= N) {
        throw std::runtime_error("Tried to access an invalid vector index.");
      } else {
        return _vec(index);
      }
    }

    /// Vector
    Eigen::Vector<double,N> _vec;
  };



  /// Calculate the modulus-squared of a vector.
  /// \f$ \sum_{i=1}^N x_i^2 \f$.
  template <size_t N>
  inline double mod2(const Vector<N>& v) {
    return v.mod2();
  }

  /// Calculate the modulus of a vector.
  /// \f$ \sqrt{\sum_{i=1}^N x_i^2} \f$.
  template <size_t N>
  inline double mod(const Vector<N>& v) {
    return v.mod();
  }


  /////////////////////////////////////////////////


  /// @name String representations of vectors
  //@{

  /// Make string representation
  template <size_t N>
  inline const string toString(const Vector<N>& v) {
    ostringstream out;
    out << "(";
    for (size_t i = 0; i < v.size(); ++i) {
      out << (fabs(v[i]) < 1E-30 ? 0.0 : v[i]);
      if (i < v.size()-1) out << ", ";
    }
    out << ")";
    return out.str();
  }

  /// Stream out string representation
  template <size_t N>
  inline std::ostream& operator<<(std::ostream& out, const Vector<N>& v) {
    out << toString(v);
    return out;
  }

  //@}


  /////////////////////////////////////////////////


  /// Compare two vectors by index, allowing for numerical precision
  template <size_t N>
  inline bool fuzzyEquals(const Vector<N>& va, const Vector<N>& vb, double tolerance=1E-5) {
    for (size_t i = 0; i < N; ++i) {
      const double a = va.get(i);
      const double b = vb.get(i);
      if (!Rivet::fuzzyEquals(a, b, tolerance)) return false;
    }
    return true;
  }


  /// External form of numerically safe nullness check
  template <size_t N>
  inline bool isZero(const Vector<N>& v, double tolerance=1E-5) {
    return v.isZero(tolerance);
  }


}

#endif
