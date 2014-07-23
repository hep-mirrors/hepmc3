#ifndef RIVET_MATH_MATRIXN
#define RIVET_MATH_MATRIXN

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/Vectors.hh"

#include "Rivet/Math/eigen/matrix.h"

namespace Rivet {


  template <size_t N>
  class Matrix;
  typedef Matrix<4> Matrix4;

  template <size_t N>
  Matrix<N> multiply(const Matrix<N>& a, const Matrix<N>& b);
  template <size_t N>
  Matrix<N> divide(const Matrix<N>&, const double);
  template <size_t N>
  Matrix<N> operator*(const Matrix<N>& a, const Matrix<N>& b);


  ///////////////////////////////////


  /// @brief General \f$ N \f$-dimensional mathematical matrix object.
  template <size_t N>
  class Matrix {
    template <size_t M>
    friend Matrix<M> add(const Matrix<M>&, const Matrix<M>&);
    template <size_t M>
    friend Matrix<M> multiply(const double, const Matrix<M>&);
    template <size_t M>
    friend Matrix<M> multiply(const Matrix<M>&, const Matrix<M>&);
    template <size_t M>
    friend Vector<M> multiply(const Matrix<M>&, const Vector<M>&);
    template <size_t M>
    friend Matrix<M> divide(const Matrix<M>&, const double);

  public:
    static Matrix<N> mkZero() {
      Matrix<N> rtn;
      return rtn;
    }

    static Matrix<N> mkDiag(Vector<N> diag) {
      Matrix<N> rtn;
      for (size_t i = 0; i < N; ++i) {
        rtn.set(i, i, diag[i]);
      }
      return rtn;
    }

    static Matrix<N> mkIdentity() {
      Matrix<N> rtn;
      for (size_t i = 0; i < N; ++i) {
        rtn.set(i, i, 1);
      }
      return rtn;
    }


  public:

    Matrix() {
      _matrix.loadZero();
    }

    Matrix(const Matrix<N>& other) {
      _matrix = other._matrix;
    }

    Matrix& set(const size_t i, const size_t j, const double value) {
      if (i < N && j < N) {
        _matrix(i, j) = value;
      } else {
        throw std::runtime_error("Attempted set access outside matrix bounds.");
      }
      return *this;
    }

    double get(const size_t i, const size_t j) const {
      if (i < N && j < N) {
        return _matrix(i, j);
      } else {
        throw std::runtime_error("Attempted get access outside matrix bounds.");
      }
    }

    Vector<N> getRow(const size_t row) const {
      Vector<N> rtn;
      for (size_t i = 0; i < N; ++i) {
        rtn.set(i, _matrix(row,i));
      }
      return rtn;
    }

    Matrix<N>& setRow(const size_t row, const Vector<N>& r) {
      for (size_t i = 0; i < N; ++i) {
        _matrix(row,i) = r.get(i);
      }
      return *this;
    }

    Vector<N> getColumn(const size_t col) const {
      Vector<N> rtn;
      for (size_t i = 0; i < N; ++i) {
        rtn.set(i, _matrix(i,col));
      }
      return rtn;
    }

    Matrix<N>& setColumn(const size_t col, const Vector<N>& c) {
      for (size_t i = 0; i < N; ++i) {
        _matrix(i,col) = c.get(i);
      }
      return *this;
    }

    Matrix<N> transpose() const {
      Matrix<N> tmp = *this;
      tmp._matrix.replaceWithAdjoint();
      return tmp;
    }

    // Matrix<N>& transposeInPlace() {
    //   _matrix.replaceWithAdjoint();
    //   return *this;
    // }

    /// Calculate inverse
    Matrix<N> inverse() const {
      Matrix<N> tmp;
      tmp._matrix = _matrix.inverse();
      return tmp;
    }

    /// Calculate determinant
    double det() const  {
      return _matrix.determinant();
    }

    /// Calculate trace
    double trace() const {
      double tr = 0.0;
      for (size_t i = 0; i < N; ++i) {
        tr += _matrix(i,i);
      }
      return tr;
      // return _matrix.trace();
    }

    /// Negate
    Matrix<N> operator-() const {
      Matrix<N> rtn;
      rtn._matrix = -_matrix;
      return rtn;
    }

    /// Get dimensionality
    size_t size() const {
      return N;
    }

    /// Index-wise check for nullness, allowing for numerical precision
    bool isZero(double tolerance=1E-5) const {
      for (size_t i=0; i < N; ++i) {
        for (size_t j=0; j < N; ++j) {
          if (! Rivet::isZero(_matrix(i,j), tolerance) ) return false;
        }
      }
      return true;
    }

    /// Check for index-wise equality, allowing for numerical precision
    bool isEqual(Matrix<N> other) const {
      for (size_t i=0; i < N; ++i) {
        for (size_t j=i; j < N; ++j) {
          if (! Rivet::isZero(_matrix(i,j) - other._matrix(i,j)) ) return false;
        }
      }
      return true;
    }

    /// Check for symmetry under transposition
    bool isSymm() const {
      return isEqual(this->transpose());
    }

    /// Check that all off-diagonal elements are zero, allowing for numerical precision
    bool isDiag() const {
      for (size_t i=0; i < N; ++i) {
        for (size_t j=0; j < N; ++j) {
          if (i == j) continue;
          if (! Rivet::isZero(_matrix(i,j)) ) return false;
        }
      }
      return true;
    }

    bool operator==(const Matrix<N>& a) const {
      return _matrix == a._matrix;
    }

    bool operator!=(const Matrix<N>& a) const {
      return _matrix != a._matrix;
    }

    bool operator<(const Matrix<N>& a) const {
      return _matrix < a._matrix;
    }

    bool operator<=(const Matrix<N>& a) const {
      return _matrix <= a._matrix;
    }

    bool operator>(const Matrix<N>& a) const {
      return _matrix > a._matrix;
    }

    bool operator>=(const Matrix<N>& a) const {
      return _matrix >= a._matrix;
    }

    Matrix<N>& operator*=(const Matrix<N>& m) {
      _matrix = _matrix * m._matrix;
      return *this;
    }

    Matrix<N>& operator*=(const double a) {
      _matrix *= a;
      return *this;
    }

    Matrix<N>& operator/=(const double a) {
      _matrix /= a;
      return *this;
    }

    Matrix<N>& operator+=(const Matrix<N>& m) {
      _matrix += m._matrix;
      return *this;
    }

    Matrix<N>& operator-=(const Matrix<N>& m) {
      _matrix -= m._matrix;
      return *this;
    }

  protected:
    typedef Eigen::Matrix<double,N> EMatrix;
    EMatrix _matrix;
  };


  /////////////////////////////////


  template <size_t N>
  inline Matrix<N> add(const Matrix<N>& a, const Matrix<N>& b) {
    Matrix<N> result;
    result._matrix = a._matrix + b._matrix;
    return result;
  }

  template <size_t N>
  inline Matrix<N> subtract(const Matrix<N>& a, const Matrix<N>& b) {
    return add(a, -b);
  }

  template <size_t N>
  inline Matrix<N> operator+(const Matrix<N> a, const Matrix<N>& b) {
    return add(a, b);
  }

  template <size_t N>
  inline Matrix<N> operator-(const Matrix<N> a, const Matrix<N>& b) {
    return subtract(a, b);
  }

  template <size_t N>
  inline Matrix<N> multiply(const double a, const Matrix<N>& m) {
    Matrix<N> rtn;
    rtn._matrix = a * m._matrix;
    return rtn;
  }

  template <size_t N>
  inline Matrix<N> multiply(const Matrix<N>& m, const double a) {
    return multiply(a, m);
  }

  template <size_t N>
  inline Matrix<N> divide(const Matrix<N>& m, const double a) {
    return multiply(1/a, m);
  }

  template <size_t N>
  inline Matrix<N> operator*(const double a, const Matrix<N>& m) {
    return multiply(a, m);
  }

  template <size_t N>
  inline Matrix<N> operator*(const Matrix<N>& m, const double a) {
    return multiply(a, m);
  }

  template <size_t N>
  inline Matrix<N> multiply(const Matrix<N>& a, const Matrix<N>& b) {
    Matrix<N> tmp;
    tmp._matrix = a._matrix * b._matrix;
    return tmp;
  }

  template <size_t N>
  inline Matrix<N> operator*(const Matrix<N>& a, const Matrix<N>& b) {
    return multiply(a, b);
  }


  template <size_t N>
  inline Vector<N> multiply(const Matrix<N>& a, const Vector<N>& b) {
    Vector<N> tmp;
    tmp._vec = a._matrix * b._vec;
    return tmp;
  }

  template <size_t N>
  inline Vector<N> operator*(const Matrix<N>& a, const Vector<N>& b) {
    return multiply(a, b);
  }

  template <size_t N>
  inline Matrix<N> transpose(const Matrix<N>& m) {
    // Matrix<N> tmp;
    // for (size_t i = 0; i < N; ++i) {
    //   for (size_t j = 0; j < N; ++j) {
    //     tmp.set(i, j, m.get(j, i));
    //   }
    // }
    // return tmp;
    return m.transpose();
  }

  template <size_t N>
  inline Matrix<N> inverse(const Matrix<N>& m) {
    return m.inverse();
  }

  template <size_t N>
  inline double det(const Matrix<N>& m) {
    return m.determinant();
  }

  template <size_t N>
  inline double trace(const Matrix<N>& m) {
    return m.trace();
  }


  /////////////////////////////////


  /// Make string representation
  template <size_t N>
  inline string toString(const Matrix<N>& m) {
    ostringstream ss;
    ss << "[ ";
    for (size_t i = 0; i < m.size(); ++i) {
      ss << "( ";
      for (size_t j = 0; j < m.size(); ++j) {
        const double e = m.get(i, j);
        ss << (Rivet::isZero(e) ? 0.0 : e) << " ";
      }
      ss << ") ";
    }
    ss << "]";
    return ss.str();
  }


  /// Stream out string representation
  template <size_t N>
  inline ostream& operator<<(std::ostream& out, const Matrix<N>& m) {
    out << toString(m);
    return out;
  }


  /////////////////////////////////////////////////


  /// Compare two matrices by index, allowing for numerical precision
  template <size_t N>
  inline bool fuzzyEquals(const Matrix<N>& ma, const Matrix<N>& mb, double tolerance=1E-5) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        const double a = ma.get(i, j);
        const double b = mb.get(i, j);
        if (!Rivet::fuzzyEquals(a, b, tolerance)) return false;
      }
    }
    return true;
  }


  /// External form of numerically safe nullness check
  template <size_t N>
  inline bool isZero(const Matrix<N>& m, double tolerance=1E-5) {
    return m.isZero(tolerance);
  }


}

#endif
