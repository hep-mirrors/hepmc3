#ifndef RIVET_MATH_MATRIXDIAG
#define RIVET_MATH_MATRIXDIAG

#include "Rivet/Math/MathHeader.hh"
#include "Rivet/Math/MatrixN.hh"

#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_eigen.h"

namespace Rivet {


template <size_t N>
class EigenSystem;
template <size_t N>
EigenSystem<N> diagonalize(const Matrix<N>& m);


/// Handy object containing results of a diagonalization.
template <size_t N>
class EigenSystem {
  template <size_t M>
  friend EigenSystem<M> diagonalize(const Matrix<M>&);

public:

  typedef pair<double, Vector<N> > EigenPair;
  typedef vector<EigenPair> EigenPairs;

  Vector<N> getDiagVector() const {
    assert(_eigenPairs.size() == N);
    Vector<N> ret;
    for (size_t i = 0; i < N; ++i) {
      ret.set(i, _eigenPairs[i].first);
    }
    return ret;
  }

  Matrix<N> getDiagMatrix() const {
    return Matrix<N>::mkDiag(getDiagVector());
  }

  EigenPairs getEigenPairs() const {
    return _eigenPairs;
  }

  vector<double> getEigenValues() const {
    assert(_eigenPairs.size() == N);
    vector<double> ret;
    for (size_t i = 0; i < N; ++i) {
      ret.push_back(_eigenPairs[i].first);
    }
    return ret;
  }

  vector<Vector<N> > getEigenVectors() const {
    assert(_eigenPairs.size() == N);
    vector<Vector<N> > ret;
    for (size_t i = 0; i < N; ++i) {
      ret.push_back(_eigenPairs[i].second);
    }
    return ret;
  }

private:

  EigenPairs _eigenPairs;

};


/// Comparison functor for "eigen-pairs".
template <size_t N>
struct EigenPairCmp :
  public std::binary_function<const typename EigenSystem<N>::EigenPair&,
                              const typename EigenSystem<N>::EigenPair&, bool> {
  bool operator()(const typename EigenSystem<N>::EigenPair& a,
                  const typename EigenSystem<N>::EigenPair& b) {
    return a.first < b.first;
  }
};


/// Diagonalize an NxN matrix, returning a collection of pairs of
/// eigenvalues and eigenvectors, ordered decreasing in eigenvalue.
template <size_t N>
EigenSystem<N> diagonalize(const Matrix<N>& m) {
  EigenSystem<N> esys;

  // Make a GSL matrix.
  gsl_matrix* A = gsl_matrix_alloc(N, N);
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      gsl_matrix_set(A, i, j, m.get(i, j));
    }
  }

  // Use GSL diagonalization.
  gsl_matrix* vecs = gsl_matrix_alloc(N, N);
  gsl_vector* vals = gsl_vector_alloc(N);
  gsl_eigen_symmv_workspace* workspace = gsl_eigen_symmv_alloc(N);
  gsl_eigen_symmv(A, vals, vecs, workspace);
  gsl_eigen_symmv_sort(vals, vecs, GSL_EIGEN_SORT_VAL_DESC);

  // Build the vector of "eigen-pairs".
  typename EigenSystem<N>::EigenPairs eigensolns;
  for (size_t i = 0; i < N; ++i) {
    typename EigenSystem<N>::EigenPair ep;
    ep.first = gsl_vector_get(vals, i);
    Vector<N> ev;
    for (size_t j = 0; j < N; ++j) {
      ev.set(j, gsl_matrix_get(vecs, j, i));
    }
    ep.second = ev;
    eigensolns.push_back(ep);
  }

  // Free GSL memory.
  gsl_eigen_symmv_free(workspace);
  gsl_matrix_free(A);
  gsl_matrix_free(vecs);
  gsl_vector_free(vals);

  // Populate the returned object.
  esys._eigenPairs = eigensolns;
  return esys;
}


template <size_t N>
inline const string toString(const typename EigenSystem<N>::EigenPair& e) {
  ostringstream ss;
  //for (typename EigenSystem<N>::EigenPairs::const_iterator i = e.begin(); i != e.end(); ++i) {
  ss << e->first << " -> " << e->second;
  //  if (i+1 != e.end()) ss << endl;
  //}
  return ss.str();
}

template <size_t N>
inline ostream& operator<<(std::ostream& out, const typename EigenSystem<N>::EigenPair& e) {
  out << toString(e);
  return out;
}


}

#endif
