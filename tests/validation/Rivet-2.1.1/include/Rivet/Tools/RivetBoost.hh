#ifndef RIVET_RIVETBOOST_HH
#define RIVET_RIVETBOOST_HH

#include "boost/smart_ptr.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/assign.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/type_traits.hpp"
#include "boost/utility.hpp"

#include "boost/foreach.hpp"
#ifndef foreach
#define foreach BOOST_FOREACH
#endif

namespace Rivet {


  // Smart pointers
  using boost::shared_ptr;

  // Clever casts
  using boost::lexical_cast;
  using boost::bad_lexical_cast;

  // Clever assignment shortcuts
  using namespace boost::assign;

  // General Boost namespace
  using namespace boost;


}

#endif
