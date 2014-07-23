// -*- C++ -*-
#ifndef RIVET_Cmp_HH
#define RIVET_Cmp_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projection.hh"
#include "Cmp.fhh"
#include <typeinfo>


namespace Rivet {


  /// Helper class when checking the ordering of two objects.
  ///
  /// Cmp is a helper class to be used when checking the ordering of two
  /// objects. When implicitly converted to an integer the value will be
  /// negative if the two objects used in the constructor are ordered and
  /// positive if they are not. Zero will be returned if they are equal.
  ///
  /// The main usage of the Cmp class is if several variables should be
  /// checked for ordering in which case several Cmp objects can be
  /// combined as follows: <code>cmp(a1, a2) || cmp(b1, b2) || cmp(c1,
  /// c2)</code> where cmp is a global function for easy creation of Cmp
  /// objects.
  template <typename T>
  class Cmp {
  public:

    /// @name Standard constructors etc.
    //@{

    /// The default constructor.
    Cmp(const T& t1, const T& t2)
      : _value(UNDEFINED), _objects(&t1, &t2) { }

    /// The copy constructor.
    template <typename U>
    Cmp(const Cmp<U>& x)
      : _value(x), _objects(0, 0) { }

    /// The destructor is not virtual since this is not intended to be a base class.
    ~Cmp() { };

    /// The assignment operator.
    template <typename U>
    const Cmp<T>& operator=(const Cmp<U>& x) {
      _value = x;
      return *this;
    }

    //@}

  public:

    /// Automatically convert to an enum.
    operator CmpState() const {
      _compare();
      return _value;
    }

    /// Automatically convert to an integer.
    operator int() const {
      _compare();
      return _value;
    }

    /// If this state is equivalent, set this state to the state of \a c.
    template <typename U>
    const Cmp<T>& operator||(const Cmp<U>& c) const {
      _compare();
      if (_value == EQUIVALENT) _value = c;
      return *this;
    }

  private:

    /// Perform the actual comparison if necessary.
    void _compare() const {
      if (_value == UNDEFINED) {
        less<T> l;
        if ( l(*_objects.first, *_objects.second) ) _value = ORDERED;
        else if ( l(*_objects.second, *_objects.first) ) _value = UNORDERED;
        else _value = EQUIVALENT;
      }
    }

    /// The state of this object.
    mutable CmpState _value;

    /// The objects to be compared.
    pair<const T*, const T*> _objects;

  };


  /// @brief Specialization of Cmp for checking the ordering of two @a {Projection}s.
  ///
  /// Specialization of the Cmp helper class to be used when checking the
  /// ordering of two Projection objects. When implicitly converted to an
  /// integer the value will be negative if the two objects used in the
  /// constructor are ordered and positive if they are not. Zero will be
  /// returned if they are equal. This specialization uses directly the
  /// virtual compare() function in the Projection class.
  ///
  /// The main usage of the Cmp class is if several variables should be
  /// checked for ordering in which case several Cmp objects can be
  /// combined as follows: <code>cmp(a1, a2) || cmp(b1, b2) || cmp(c1,
  /// c2)</code> where cmp is a global function for easy creation of Cmp
  /// objects.
  template <>
  class Cmp<Projection> {
  public:

    /// @name Standard constructors and destructors.
    //@{
    /// The default constructor.
    Cmp(const Projection& p1, const Projection& p2)
      : _value(UNDEFINED), _objects(&p1, &p2)
    { }

    /// The copy constructor.
    template <typename U>
    Cmp(const Cmp<U>& x)
      : _value(x), _objects(0, 0)
    { }

    /// The destructor is not virtual since this is not intended to be a base class.
    ~Cmp() { };

    /// The assignment operator.
    template <typename U>
    const Cmp<Projection>& operator=(const Cmp<U>& x) {
      _value = x;
      return *this;
    }
    //@}

  public:

    /// Automatically convert to an enum.
    operator CmpState() const {
      _compare();
      return _value;
    }


    /// Automatically convert to an integer.
    operator int() const {
      _compare();
      return _value;
    }

    /// If this state is equivalent, set this state to the state of \a c.
    template <typename U>
    const Cmp<Projection>& operator||(const Cmp<U>& c) const {
      _compare();
      if (_value == EQUIVALENT) _value = c;
      return *this;
    }

  private:

    /// Perform the actual comparison if necessary.
    void _compare() const {
      if (_value == UNDEFINED) {
        const std::type_info& id1 = typeid(*_objects.first);
        const std::type_info& id2 = typeid(*_objects.second);
        if (id1.before(id2)) _value = ORDERED;
        else if (id2.before(id1)) _value = UNORDERED;
        else {
          int c = _objects.first->compare(*_objects.second);
          if (c < 0) _value = ORDERED;
          else if (c > 0) _value = UNORDERED;
          else _value = EQUIVALENT;
        }
      }
    }

  private:

    /// The state of this object.
    mutable CmpState _value;

    /// The objects to be compared.
    pair<const Projection*, const Projection*> _objects;

  };




  /// @brief Specialization of Cmp for checking the ordering of two floating point numbers.
  ///
  /// When implicitly converted to an integer the value will be negative if the
  /// two objects used in the constructor are ordered and positive if they are
  /// not. Zero will be returned if they are equal. This specialization uses the
  /// Rivet fuzzyEquals function to indicate equivalence protected from
  /// numerical precision effects.
  ///
  /// The main usage of the Cmp class is if several variables should be
  /// checked for ordering in which case several Cmp objects can be
  /// combined as follows: <code>cmp(a1, a2) || cmp(b1, b2) || cmp(c1,
  /// c2)</code> where cmp is a global function for easy creation of Cmp
  /// objects.
  template <>
  class Cmp<double> {
  public:

    /// @name Standard constructors and destructors.
    //@{
    /// The default constructor.
    Cmp(const double p1, const double p2)
      : _value(UNDEFINED), _numA(p1), _numB(p2)
    { }

    /// The copy constructor.
    template <typename U>
    Cmp(const Cmp<U>& x)
      : _value(x), _numA(0.0), _numB(0.0)
    { }

    /// The destructor is not virtual since this is not intended to be a base class.
    ~Cmp() { }

    /// The assignment operator.
    template <typename U>
    const Cmp<double>& operator=(const Cmp<U>& x) {
      _value = x;
      return *this;
    }
    //@}

  public:

    /// Automatically convert to an enum.
    operator CmpState() const {
      _compare();
      return _value;
    }

    /// Automatically convert to an integer.
    operator int() const {
      _compare();
      return _value;
    }

    /// If this state is equivalent, set this state to the state of \a c.
    template <typename U>
    const Cmp<double>& operator||(const Cmp<U>& c) const {
      _compare();
      if (_value == EQUIVALENT) _value = c;
      return *this;
    }

  private:

    /// Perform the actual comparison if necessary.
    void _compare() const {
      if (_value == UNDEFINED) {
        if (fuzzyEquals(_numA,_numB)) _value = EQUIVALENT;
        else if (_numA < _numB) _value = ORDERED;
        else _value = UNORDERED;
      }
    }

  private:

    /// The state of this object.
    mutable CmpState _value;

    /// The objects to be compared.
    double _numA, _numB;

  };



  ///////////////////////////////////////////////////////////////////



  /// Global helper function for easy creation of Cmp objects.
  template <typename T>
  inline Cmp<T> cmp(const T& t1, const T& t2) {
    return Cmp<T>(t1, t2);
  }


  /// Typedef for Cmp<Projection>
  typedef Cmp<Projection> PCmp;


  /// Global helper function for easy creation of Cmp<Projection> objects.
  inline Cmp<Projection> pcmp(const Projection& p1, const Projection& p2) {
    return Cmp<Projection>(p1, p2);
  }

  /// Global helper function for easy creation of Cmp<Projection> objects from
  /// two parent projections and their common name for the projection to be compared.
  inline Cmp<Projection> pcmp(const Projection& parent1, const Projection& parent2, const string& pname) {
    return Cmp<Projection>(parent1.getProjection(pname), parent2.getProjection(pname));
  }

  /// Global helper function for easy creation of Cmp<Projection> objects from
  /// two parent projections and their common name for the projection to be compared.
  /// This version takes one parent as a pointer.
  inline Cmp<Projection> pcmp(const Projection* parent1, const Projection& parent2, const string& pname) {
    assert(parent1);
    return Cmp<Projection>(parent1->getProjection(pname), parent2.getProjection(pname));
  }

  /// Global helper function for easy creation of Cmp<Projection> objects from
  /// two parent projections and their common name for the projection to be compared.
  /// This version takes one parent as a pointer.
  inline Cmp<Projection> pcmp(const Projection& parent1, const Projection* parent2, const string& pname) {
    assert(parent2);
    return Cmp<Projection>(parent1.getProjection(pname), parent2->getProjection(pname));
  }

  /// Global helper function for easy creation of Cmp<Projection> objects from
  /// two parent projections and their common name for the projection to be compared.
  inline Cmp<Projection> pcmp(const Projection* parent1, const Projection* parent2, const string& pname) {
    assert(parent1);
    assert(parent2);
    return Cmp<Projection>(parent1->getProjection(pname), parent2->getProjection(pname));
  }


}


#endif
