#ifndef  HEPMC3_GENPARTICLE_H
#define  HEPMC3_GENPARTICLE_H

#include <iostream>
#include "HepMC3/FourVector.h"

namespace HepMC3 {

class GenParticle {
//
// Constructors
//
public:
    GenParticle();
    GenParticle(FourVector momentum, int pdgid, int status);
    ~GenParticle();

//
// Functions
//
public:
    void print( std::ostream& ostr = std::cout, int format = 0 ) const;

//
// Accessors
//
public:
    int    pdg_id()                           const { return m_pdgid; }
    int    status()                           const { return m_status; }
    int    barcode()                          const { return m_barcode; }
    int    production_vertex_barcode()        const { return m_production_vertex; }
    int    end_vertex_barcode()               const { return m_end_vertex; }
    const  FourVector& momentum()             const { return m_momentum; }
    double generated_mass()                   const { return m_generated_mass; }

    void set_pdg_id(int id)                         { m_pdgid = id; }
    void set_status(int status)                     { m_status = status; }
    void set_barcode(int barcode)                   { m_barcode = barcode; }
    void set_production_vertex_barcode(int barcode) { m_production_vertex = barcode; }
    void set_end_vertex_barcode(int barcode)        { m_end_vertex = barcode; }
    void set_momentum(FourVector& momentum)         { m_momentum  = momentum; }
    void set_generated_mass(double m)               { m_generated_mass = m; }

//
// Fields
//
private:
    FourVector m_momentum;
    int m_pdgid;
    int m_status;
    int m_barcode;
    int m_production_vertex;
    int m_end_vertex;
    double m_generated_mass;
};

} // namespace HepMC3

#endif
