#ifndef  HEPMC_DATA_SMARTPOINTER_H
#define  HEPMC_DATA_SMARTPOINTER_H
/**
 *  @file SmartPointer.h
 *  @brief Definition of \b template \b class SmartPointer
 *
 *  @class HepMC::SmartPointer
 *  @brief Smart pointer for HepMC objects
 *
 *  Uses shared_ptr to manage the object memory
 *
 *  @note SmartPointer can be created from raw pointer. This allows
 *        for implicit conversions when passing raw pointer as argument to
 *        the constructors or other functions in HepMC classes for backward compatibility.
 *        SmartPointer ensures only one shared_ptr manages
 *        the object in such conversions. Note, however, that use of such conversion is deprecated.
 *
 *  @note Requires managed class to have member field \c weah_ptr<T> \c \b m_this
 *        used to keep track of shared pointer created to manage the object
 *
 *  @ingroup data
 *
 */
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace HepMC {

using boost::weak_ptr;
using boost::shared_ptr;
using boost::make_shared;

template<class T>
class SmartPointer {
//
// Constructors
//
public:
    /** @brief Default constructor (NULL pointer) */
    SmartPointer();

    /** @brief Copy constructor */
    SmartPointer( const SmartPointer<T> &rhs );

    /** @brief Construct SmartPointer using shared pointer
     *
     *  @note It's advised not to use shared_ptr<T> when using SmartPointer
     *        This constructor should be used only in combination with make_shared<T>
     */
    SmartPointer( const shared_ptr<T> &rhs );

    /** @brief Constructor creating shared pointer from raw pointer */
    SmartPointer( T *raw_pointer );

//
// Accessors
//
public:
    SmartPointer& operator=(const SmartPointer &rhs) { m_data = rhs.m_data; return *this; } //!< Assignment operator

    bool operator==(const SmartPointer &rhs)  const { return  m_data == rhs.m_data; } //!< == operator
    bool operator!=(const SmartPointer &rhs)  const { return  m_data != rhs.m_data; } //!< != operator

    const shared_ptr<T>& operator->()         const { return  m_data; } //!< Shared pointer access operator
    T&                   operator*()          const { return *m_data; } //!< Dereference operator

    operator bool() const { return m_data; } //!< Bool cast operator

//
// Deprecated functions
//
public:
    __attribute__((deprecated("Use smart pointers instead of raw pointers"))) operator T*() const { return m_data.get(); } //!< Cast to raw pointer @deprecated Should not be used at all
//
// Fields
//
private:
    shared_ptr<T> m_data; //!< Shared pointer
};

typedef SmartPointer<class GenVertex>   GenVertexPtr;   //!< Smart pointer to GenVertex
typedef SmartPointer<class GenParticle> GenParticlePtr; //!< Smart pointer to GenParticle

typedef shared_ptr<class GenPdfInfo>      GenPdfInfoPtr;      //!< Shared pointer to GenPdfInfo
typedef shared_ptr<class GenHeavyIon>     GenHeavyIonPtr;     //!< Shared pointer to GenPdfInfo
typedef shared_ptr<class GenCrossSection> GenCrossSectionPtr; //!< Shared pointer to GenPdfInfo

} // namespace HepMC

#include "HepMC/Data/SmartPointer.icc"

#endif
