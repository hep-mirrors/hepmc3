#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H

#include <iostream>
#include <vector>
#include "HepMC3/GenParticle.h"
using std::vector;

namespace HepMC3 {

class GenVertex {
  //
  // Constructors
  //
public:
  GenVertex();
  ~GenVertex();

  void print( std::ostream& ostr = std::cout ) const;

  //
  // Functions
  //
public:
  int barcode()                        const { return m_barcode; }

  vector<GenParticle*>& particles_in()       { return m_particles_in; }
  vector<GenParticle*>& particles_out()      { return m_particles_out; }

  void add_particle_in (GenParticle *p);
  void add_particle_out(GenParticle *p);

  //
  // Accessors
  //
public:
  void set_barcode(int barcode)              { m_barcode = barcode; }

  //
  // Fields
  //
private:
  int m_barcode;

  vector<GenParticle*> m_particles_in;
  vector<GenParticle*> m_particles_out;
};

} // namespace HepMC3

#endif
