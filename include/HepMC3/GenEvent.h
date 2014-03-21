#ifndef  HEPMC3_GENEVENT_H
#define  HEPMC3_GENEVENT_H

#include <vector>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
using std::vector;

namespace HepMC3 {

class GenEvent {
//
// Constructors
//
public:
    GenEvent();
    ~GenEvent();

//
// Functions
//
public:
    void print( std::ostream& ostr = std::cout, int format = 1 ) const;

//
// Accessors
//
public:
    int event_number() const              { return m_event_number; }

    vector<GenParticle*>& particles()             { return m_particles; }
    vector<GenVertex*>&   vertices()              { return m_vertices; }
    const vector<GenParticle*>& particles() const { return m_particles; }
    const vector<GenVertex*>&   vertices()  const { return m_vertices; }

    void set_event_number(int no)         { m_event_number = no; }
    void add_particle(GenParticle *p);
    void add_vertex(GenVertex *v);

//
// Fields
//
private:
    int m_event_number;
    int m_lowest_barcode;
    int m_lowest_vertex;

    vector<GenParticle*> m_particles;
    vector<GenVertex*>   m_vertices;
};

} // namespace HepMC3

#endif
