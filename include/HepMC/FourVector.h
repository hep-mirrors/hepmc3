#ifndef  HEPMC_FOURVECTOR_H
#define  HEPMC_FOURVECTOR_H
/**
 *  @file FourVector.h
 *  @brief Definition of \b class FourVector
 *
 *  @class HepMC::FourVector
 *  @brief Generic 4-vector
 *
 *  Interpretation of its content depends on accessors used.
 *  Contains few support operations on 4-vectors and floating-point values.
 *  Fully inlined
 *
 *  @todo Change notation to eta,phi,pT. Make proper accessors
 *
 */
#include "HepMC/Setup.h"

namespace HepMC {

class FourVector {
//
// Constructors
//
public:
    /** @brief Default constructor */
    FourVector()                                      :v1(0.0), v2(0.0), v3(0.0), v4(0.0)  {}
    /** @brief Sets all FourVector fields */
    FourVector(double x, double y, double z, double e):v1(x),   v2(y),   v3(z),   v4(e)    {}
    /** @brief Copy constructor */
    FourVector(const FourVector & v)                  :v1(v.v1),v2(v.v2),v3(v.v3),v4(v.v4) {}

//
// Functions
//
public:

    double m()       const; //!< Calculate mass. Returns -sqrt(-m) if e^2 - P^2 is negative
    double length()  const; //!< Calculate length
    bool   is_zero() const; //!< Check if the length of this vertex is equivalent to zero

    bool        operator==(const FourVector& rhs) const;                            //!< Boolean operator ==
    bool        operator!=(const FourVector& rhs) const { return !(*this == rhs); } //!< Boolean operator !=

    FourVector  operator+ (const FourVector& rhs) const; //!< Arithmetic operator +
    FourVector  operator- (const FourVector& rhs) const; //!< Arithmetic operator -
    FourVector  operator* (const double rhs)      const; //!< Arithmetic operator *
    FourVector  operator/ (const double rhs)      const; //!< Arithmetic operator /

    void        operator+=(const FourVector& rhs);       //!< Assignment operator +=
    void        operator-=(const FourVector& rhs);       //!< Assignment operator -=
    void        operator*=(const double rhs);            //!< Assignment operator *=
    void        operator/=(const double rhs);            //!< Assignment operator /=

//
// Functions related to floating-point operations
//
public:
    /** @brief Check if floating-point value is almost equal 0 */
    static bool AlmostEqualZero(double A);

    /** @brief Compare floating-point numbers
     *
     *  Bruce Dawson algorithm for comparing floating-point numbers
     *  Implementation for 64-bit double precision
     *  Updated to remove strict-aliasing warnings
     *
     *  @see https://randomascii.wordpress.com/category/floating-point/
     *
     *  @param A first value
     *  @param B second value
     *  @param maxUlps maximum acceptable deviation in number of units in the last place
     *                 maxUlps > 0 && maxUlps < 4 * 1024 * 1024
     */
    static bool AlmostEqual2sComplement(double A, double B, unsigned int maxUlps = Setup::DEFAULT_DOUBLE_ALMOST_EQUAL_MAXULPS);

//
// Accessors
//
public:
    // As 4-momentum
    double px()       const { return v1; } //!< Get px
    void   setPx(double px) { v1 = px;   } //!< Set px

    double py()       const { return v2; } //!< Get py
    void   setPy(double py) { v2 = py;   } //!< Set py

    double pz()       const { return v3; } //!< Get pz
    void   setPz(double pz) { v3 = pz;   } //!< Set pz

    double e()        const { return v4; } //!< Get energy
    void   setE (double e ) { v4 = e;    } //!< Set energy

    // As time-space
    double x()        const { return v1; } //!< Get x coordinate
    void   setX(double x)   { v1 = x;    } //!< Set x coordinate

    double y()        const { return v2; } //!< Get y coordinate
    void   setY(double y)   { v2 = y;    } //!< Set y coordinate

    double z()        const { return v3; } //!< Get z coordinate
    void   setZ(double z)   { v3 = z;    } //!< Set z coordinate

    double t()        const { return v4; } //!< Get time
    void   setT(double t)   { v4 = t;    } //!< Set time

    /** @brief Set all FourVector fields */
    void set(double x1, double x2, double x3, double x4) {
        v1 = x1;
        v2 = x2;
        v3 = x3;
        v4 = x4;
    }

//
// Fields
//
public:
    static const FourVector& ZERO_VECTOR(); //!< FourVector(0,0,0,0)
private:
    double v1; // px or x. Interpretation depends on accessors used
    double v2; // py or y. Interpretation depends on accessors used
    double v3; // pz or z. Interpretation depends on accessors used
    double v4; //e  or t. Interpretation depends on accessors used
};

} // namespace HepMC

#include "HepMC/FourVector.icc"

#endif
