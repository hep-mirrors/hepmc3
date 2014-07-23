#ifndef RIVET_Math_MathHeader
#define RIVET_Math_MathHeader

#include "Rivet/Exceptions.hh"
#include <stdexcept>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>


// Macro to help with overzealous compiler warnings
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


namespace Rivet {

  using std::string;
  using std::ostream;
  using std::ostringstream;
  using std::cout;
  using std::endl;
  using std::pair;
  using std::vector;
  using std::transform;
  using std::min;
  using std::max;
  using std::abs;
  using std::isnan;
  using std::isinf;

  const double MAXDOUBLE = std::numeric_limits<double>::max();
  const double MAXINT = std::numeric_limits<int>::max();

  /// A pre-defined value of \f$ \pi \f$.
  const double PI = M_PI;

  /// A pre-defined value of \f$ 2\pi \f$.
  const double TWOPI = 2*M_PI;

  /// A pre-defined value of \f$ \pi/2 \f$.
  const double HALFPI = M_PI_2;

  /// Enum for signs of numbers.
  enum Sign { MINUS = -1, ZERO = 0, PLUS = 1 };

  /// Enum for rapidity variable to be used in calculating \f$ R \f$, applying rapidity cuts, etc.
  enum RapScheme { PSEUDORAPIDITY = 0, ETARAP = 0, RAPIDITY = 1, YRAP = 1 };

  /// Enum for range of \f$ \phi \f$ to be mapped into
  enum PhiMapping { MINUSPI_PLUSPI, ZERO_2PI, ZERO_PI };

}

#endif
