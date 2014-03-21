#ifndef  HEPMC3_FOURVECTOR_H
#define  HEPMC3_FOURVECTOR_H

#include <cmath>

namespace HepMC3 {

class FourVector {
//
// Constructors
//
public:
    FourVector()                                      :m_px(0.0),   m_py(0.0),   m_pz(0.0),   m_e(0.0)   {}
    FourVector(double x, double y, double z, double e):m_px(x),     m_py(y),     m_pz(z),     m_e(e)     {}
    FourVector(const FourVector & v)                  :m_px(v.px()),m_py(v.py()),m_pz(v.pz()),m_e(v.e()) {} //! copy constructor

//
// Functions
//
public:
    double m() const {
        double m = m_e*m_e - m_px*m_px - m_py*m_py - m_pz*m_pz;
        if(m<0.0) return -sqrt(-m);
        return sqrt(m);
    }

//
// Accessors
//
public:
    double px() const { return m_px; }
    double py() const { return m_py; }
    double pz() const { return m_pz; }
    double e()  const { return m_e; }

    void setPx(double xin) { m_px=xin; }
    void setPy(double yin) { m_py=yin; }
    void setPz(double zin) { m_pz=zin; }
    void setE (double ein) { m_e=ein; }

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
    double m_px;
    double m_py;
    double m_pz;
    double m_e;
};

} // namespace HepMC3

#endif
