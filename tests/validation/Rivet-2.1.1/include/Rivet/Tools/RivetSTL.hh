#ifndef RIVET_RivetSTL_HH
#define RIVET_RivetSTL_HH

#include <typeinfo>
#include <set>
#include <list>
#include <map>
#include <utility>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cassert>
#include <fstream>
#include <algorithm>

namespace Rivet {

  /// @name Convenient imports of common STL classes and functions
  //@{

  using std::set;
  using std::map;
  using std::multimap;
  using std::type_info;
  using std::string;
  using std::stringstream;
  using std::less;
  using std::list;
  using std::vector;
  using std::pair;
  using std::make_pair;
  using std::runtime_error;
  using std::min;
  using std::max;
  using std::abs;
  using std::numeric_limits;
  using std::ostream;
  using std::istream;
  using std::cout;
  using std::cin;
  using std::cerr;
  using std::setw;
  using std::pow;
  using std::endl;

  //@}


  /// @name Streaming containers as string reps
  /// @todo Make these named toStr rather than operator<<
  //@{

  /// Convenient function for streaming out vectors of any streamable object.
  template<typename T>
  inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[ ";
    for (size_t i=0; i<vec.size(); ++i) {
      os << vec[i] << " ";
    }
    os << "]";
    return os;
  }

  /// Convenient function for streaming out lists of any streamable object.
  template<typename T>
  inline std::ostream& operator<<(std::ostream& os, const std::list<T>& vec) {
    os << "[ ";
    for (size_t i=0; i<vec.size(); ++i) {
      os << vec[i] << " ";
    }
    os << "]";
    return os;
  }

  //@}

}


namespace std {

  /// @name Standard library enhancements
  /// @todo Merge in nice ideas from mili: https://code.google.com/p/mili/
  //@{

  /// @name Boolean-return container searching
  //@{

  /// Does @a s contain @a sub as a substring?
  inline bool contains(const string& s, const string& sub) {
    return s.find(sub) != string::npos;
  }

  /// Does the vector @a v contain @a x?
  template <typename T>
  inline bool contains(const vector<T>& v, const T& x) {
    return find(v.begin(), v.end(), x) != v.end();
  }

  /// Does the list @a l contain @a x?
  template <typename T>
  inline bool contains(const list<T>& l, const T& x) {
    return find(l.begin(), l.end(), x) != l.end();
  }

  /// Does the set @a s contain @a x?
  template <typename T>
  inline bool contains(const set<T>& s, const T& x) {
    return find(s.begin(), s.end(), x) != s.end();
  }

  /// Does the map @a m contain the key @a key?
  template <typename K, typename T>
  inline bool has_key(const map<K, T>& m, const K& key) {
    return m.find(key) != m.end();
  }

  /// Does the map @a m contain the value @a val?
  template <typename K, typename T>
  inline bool has_value(const map<K, T>& m, const T& val) {
    for (typename map<K,T>::const_iterator it = m.begin(); it != m.end(); ++it) {
      if (it->second == val) return true;
    }
    return false;
  }

  //@}


  /// @name Container filling and merging
  //@{

  /// @note This is provided by Boost Assignment
  // /// Append an item @a t to vector @a v
  // template <typename T>
  // inline void operator+=(vector<T>& v, const T& t) {
  //   v.push_back(t);
  // }

  /// Append all the items from vector @a v2 to vector @a v1
  template <typename T>
  inline void operator+=(vector<T>& v1, const vector<T>& v2) {
    for (typename vector<T>::const_iterator s = v2.begin(); s != v2.end(); ++s) {
      v1.push_back(*s);
    }
  }

  /// Create a new vector from the concatenated items in vectors @a v1 and @a v2
  template <typename T>
  inline vector<T> operator+(const vector<T>& v1, const vector<T>& v2) {
    vector<T> rtn(v1);
    rtn += v2;
    return rtn;
  }


  /// @note This is provided by Boost Assignment
  // /// Add an item @t to set @a s
  // template <typename T>
  // inline void operator+=(set<T>& s, const T& t) {
  //   s.insert(t);
  // }

  /// Merge the contents of set @a s2 into @a s1
  template <typename T>
  inline void operator+=(set<T>& s1, const set<T>& s2) {
    for (typename set<T>::const_iterator s = s2.begin(); s != s2.end(); ++s) {
      s1.insert(*s);
    }
  }

  /// Merge the contents of sets @a s1 and @a s2
  template <typename T>
  inline set<T> operator+(const set<T>& s1, const set<T>& s2) {
    set<T> rtn(s1);
    rtn += s2;
    return rtn;
  }

  //@}

  //@}


}

#endif
