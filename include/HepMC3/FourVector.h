#ifndef  HEPMC3_FOURVECTOR_H
#define  HEPMC3_FOURVECTOR_H
/**
 *  @file FourVector.h
 *  @brief Definition of \b class HepMC3::FourVector
 *
 *  @class HepMC3::FourVector
 *  @brief Stores 4-vector information
 *
 *  Contains few support operations on 4-vectors.
 *  Fully inlined
 *
 */
#include <cmath>

namespace HepMC3 {

class FourVector {
//
// Constructors
//
public:
    /** Default constructor */
    FourVector()                                      :m_px(0.0),   m_py(0.0),   m_pz(0.0),   m_e(0.0)   {}
    /** Sets all FourVector fields */
    FourVector(double x, double y, double z, double e):m_px(x),     m_py(y),     m_pz(z),     m_e(e)     {}
    /** Copy constructor */
    FourVector(const FourVector & v)                  :m_px(v.px()),m_py(v.py()),m_pz(v.pz()),m_e(v.e()) {}

//
// Functions
//
public:
    /** Calculate mass
     *  Returns -sqrt(-m) if e^2 - P^2 is negative
     */
    double m() const {
        double m = m_e*m_e - m_px*m_px - m_py*m_py - m_pz*m_pz;
        if(m<0.0) return -sqrt(-m);
        return sqrt(m);
    }

//
// Accessors
//
public:
    double px()        const { return m_px; } //!< Get px
    void   setPx(double xin) { m_px=xin;    } //!< Set px

    double py()        const { return m_py; } //!< Get py
    void   setPy(double yin) { m_py=yin;    } //!< Set py

    double pz()        const { return m_pz; } //!< Get pz
    void   setPz(double zin) { m_pz=zin;    } //!< Set pz

    double e()         const { return m_e;  } //!< Get energy
    void   setE (double ein) { m_e=ein;     } //!< Set energy

    /** Set all FourVector fields */
    void set(double x, double y, double z, double e) {
        m_px=x;
        m_py=y;
        m_pz=z;
        m_e=e;
    }

//
// Fields
//
private:
    /** @todo Change notation to eta,phi,pT. Make proper accessors */
    /** @todo Make FourVector able to represent (x,T) */
    double m_px; //!< px
    double m_py; //!< py
    double m_pz; //!< pz
    double m_e;  //!< e
};

} // namespace HepMC3

#endif
