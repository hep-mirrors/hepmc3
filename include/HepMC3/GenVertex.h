#ifndef  HEPMC3_GENVERTEX_H
#define  HEPMC3_GENVERTEX_H

#include <iostream>
#include <vector>
using std::vector;

namespace HepMC3 {

class GenParticle;

class GenVertex {
//
// Constructors
//
public:
    GenVertex();
    ~GenVertex();


//
// Functions
//
public:
    /** Prints information about the vertex
        By default prints only particle-related information
        event_listing_format = true is used by event for formatted output */
    void print( std::ostream& ostr = std::cout, bool event_listing_format = false ) const;

    void add_particle_in (GenParticle *p);
    void add_particle_out(GenParticle *p);

//
// Accessors
//
public:
    int barcode()                               const { return m_barcode; }
    void set_barcode(int barcode)                     { m_barcode = barcode; }

    vector<GenParticle*>& particles_in()              { return m_particles_in; }
    vector<GenParticle*>& particles_out()             { return m_particles_out; }
    const vector<GenParticle*>& particles_in()  const { return m_particles_in; }
    const vector<GenParticle*>& particles_out() const { return m_particles_out; }
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
