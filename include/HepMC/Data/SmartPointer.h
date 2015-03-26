// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/// @brief Definition of \b template \b class SmartPointer

#ifndef  HEPMC_DATA_SMARTPOINTER_H
#define  HEPMC_DATA_SMARTPOINTER_H

#include "HepMC/Common.h"

#ifdef HEPMC_HAS_CXX11

#include <memory>
namespace HepMC {
    using std::weak_ptr;
    using std::shared_ptr;
    using std::make_shared;
    using std::dynamic_pointer_cast;
    using std::const_pointer_cast;
}

#else

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace HepMC {
    using boost::weak_ptr;
    using boost::shared_ptr;
    using boost::make_shared;
    using boost::dynamic_pointer_cast;
    using boost::const_pointer_cast;
}

#endif


namespace HepMC {


    /// @class HepMC::SmartPointer
    /// @brief Smart pointer for HepMC objects
    ///
    /// Uses shared_ptr to manage the object memory
    ///
    /// @note SmartPointer can be created from raw pointer. This allows
    ///       for implicit conversions when passing raw pointer as argument to
    ///       the constructors or other functions in HepMC classes for backward compatibility.
    ///       SmartPointer ensures only one shared_ptr manages
    ///       the object in such conversions. Note, however, that use of such conversion is deprecated.
    ///
    /// @note Requires managed class to have member field \c weah_ptr<T> \c \b m_this
    ///       used to keep track of shared pointer created to manage the object
    ///
    /// @ingroup data
    ///
    template<class T>
    class SmartPointer {
    public:

        /// @name Constructors
        //@{

        /// Default constructor (NULL pointer)
        SmartPointer();

        /// Copy constructor
        SmartPointer( const SmartPointer<T> &rhs );

        /// Construct SmartPointer using shared pointer
        ///
        /// @note It's advised not to use shared_ptr<T> when using SmartPointer
        /// @note This constructor should be used only in combination with make_shared<T>
        SmartPointer( const shared_ptr<T> &rhs );

        /// Constructor creating shared pointer from raw pointer
        SmartPointer( T *raw_pointer );

        //@}


        /// @name Accessors
        //@{

        SmartPointer& operator=(const SmartPointer &rhs) { m_data = rhs.m_data; return *this; } //!< Assignment operator

        bool operator==(const SmartPointer &rhs)  const { return  m_data == rhs.m_data; } //!< == operator
        bool operator!=(const SmartPointer &rhs)  const { return  m_data != rhs.m_data; } //!< != operator

        const shared_ptr<T> operator->()                { return  m_data; } //!< Shared pointer access operator
        T&                   operator*()                { return *m_data; } //!< Dereference operator

        const shared_ptr<const T> operator->()    const { return  const_pointer_cast<const T>(m_data); } //!< Const shared pointer access operator
        const T&             operator*()          const { return *m_data; } //!< Const dereference operator

        operator bool() const { return (bool) m_data; } //!< Bool cast operator

        //@}


        /// @name Deprecated functions
        //@{

        #ifndef HEPMC_NO_DEPRECATED

        /// Cast to raw pointer
        /// @deprecated Should not be used at all
        HEPMC_DEPRECATED("Use smart pointers instead of raw pointers")
        operator T*() { return m_data.get(); }

        /// Cast to bool
        operator bool() { return (bool) m_data.get(); }

        #endif

        //@}


    private:

        /// @name Fields
        //@{
        /// Shared pointer
        shared_ptr<T> m_data;
        //@}

    };


    /// @name Typedefs for smart pointers to HepMC classes
    //@{
    typedef SmartPointer<class GenParticle> GenParticlePtr; //!< Smart pointer to GenParticle
    typedef SmartPointer<class GenVertex>   GenVertexPtr;   //!< Smart pointer to GenVertex

    typedef SmartPointer<const class GenParticle> ConstGenParticlePtr; //!< Const smart pointer to GenParticle
    typedef SmartPointer<const class GenVertex>   ConstGenVertexPtr;   //!< Const smart pointer to GenVertex

    typedef shared_ptr<struct GenPdfInfo>      GenPdfInfoPtr;      //!< Shared pointer to GenPdfInfo
    typedef shared_ptr<struct GenHeavyIon>     GenHeavyIonPtr;     //!< Shared pointer to GenHeavyIon
    typedef shared_ptr<struct GenCrossSection> GenCrossSectionPtr; //!< Shared pointer to GenCrossSection
    //@}


} // namespace HepMC

#include "HepMC/Data/SmartPointer.icc"

#endif
