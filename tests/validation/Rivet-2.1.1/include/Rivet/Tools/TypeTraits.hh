// -*- C++ -*-
#ifndef RIVET_TypeTraits_HH
#define RIVET_TypeTraits_HH

namespace Rivet {

  /// Mechanisms to allow references and pointers to templated types
  /// to be distinguished from one another (since C++ doesn't allow
  /// partial template specialisation for functions.
  /// Traits methods use specialisation of class/struct templates, and
  /// some trickery with typedefs and static const integral types (or
  /// enums) to implement partial function specialisation as a work-around.

  /// @cond INTERNAL

  struct RefType { };

  struct PtrType { };

  template <typename T>
  struct TypeTraits;

  template <typename U>
  struct TypeTraits<const U&> {
    typedef RefType ArgType;
  };

  template <typename U>
  struct TypeTraits<const U*> {
    typedef PtrType ArgType;
  };

  /// @endcond

}

#endif
