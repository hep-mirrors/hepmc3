// -*- C++ -*-
#ifndef RIVET_RivetHepMC_HH
#define RIVET_RivetHepMC_HH

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/IO/IO_GenEvent.h"
#include "Rivet/Tools/RivetSTL.hh"
#include "Rivet/Exceptions.hh"

namespace Rivet {


  using HepMC3::GenEvent;
  using HepMC3::GenParticle;
  using HepMC3::GenParticlePtr;
  using HepMC3::GenVertex;
  using HepMC3::GenVertexPtr;

  /// @todo Add functions from mcutils


  inline std::vector<GenParticlePtr> particles(const GenEvent& ge) {
    std::vector<GenParticlePtr> rtn;
    for (GenEvent::particle_const_iterator pi = ge.particles_begin(); pi != ge.particles_end(); ++pi) {
      rtn.push_back(*pi);
    }
    return rtn;
  }
  inline std::vector<GenParticlePtr> particles(const GenEvent* ge) {
    assert(ge);
    return particles(*ge);
  }
  //  inline std::pair<GenEvent::particle_const_iterator, GenEvent::particle_const_iterator> particles(const GenEvent& ge) {
  //    return make_pair(ge.particles_begin(), ge.particles_end());
  //  }
  //  inline std::pair<GenEvent::particle_const_iterator, GenEvent::particle_const_iterator> particles(const GenEvent* ge) {
  //    assert(ge);
  //    return particles(*ge);
  //  }



  inline std::vector<GenVertexPtr> vertices(const GenEvent& ge) {
    std::vector<GenVertexPtr> rtn;
    for (GenEvent::vertex_const_iterator vi = ge.vertices_begin(); vi != ge.vertices_end(); ++vi) {
      rtn.push_back(*vi);
    }
    return rtn;
  }
  inline std::vector<GenVertexPtr> vertices(const GenEvent* ge) {
    assert(ge);
    return vertices(*ge);
  }
  //  inline std::pair<GenEvent::vertex_const_iterator, GenEvent::vertex_const_iterator> vertices(const GenEvent& ge) {
  //    return make_pair(ge.vertices_begin(), ge.vertices_end());
  //  }
  //  inline std::pair<GenEvent::vertex_const_iterator, GenEvent::vertex_const_iterator> vertices(const GenEvent* ge) {
  //    assert(ge);
  //    return vertices(*ge);
  //  }


  ////////////////////////////////////


  inline std::pair<GenVertex::particles_in_const_iterator, GenVertex::particles_in_const_iterator>
  particles_in(const GenVertexPtr gv) {
    return make_pair(gv->particles_in_const_begin(), gv->particles_in_const_end());
  }


  inline std::pair<GenVertex::particles_out_const_iterator, GenVertex::particles_out_const_iterator>
  particles_out(const GenVertexPtr gv) {
    return make_pair(gv->particles_out_const_begin(), gv->particles_out_const_end());
  }


  inline std::vector<GenParticlePtr> particles(GenVertexPtr gv, HepMC3::IteratorRange range=HepMC3::relatives) {
    std::vector<GenParticlePtr> rtn;
    for (GenVertex::particle_iterator pi = gv->particles_begin(range); pi != gv->particles_end(range); ++pi) {
      rtn.push_back(*pi);
    }
    return rtn;
  }
  //  inline std::pair<GenVertex::particle_iterator, GenVertex::particle_iterator> particles(GenVertexPtr gv, HepMC3::IteratorRange range=HepMC3::relatives) {
  //    return make_pair(gv->particles_begin(range), gv->particles_end(range));
  //  }


  //////////////////////////


  /// Get the direct parents or all-ancestors of GenParticle @a gp
  inline std::vector<GenParticlePtr> particles_in(const GenParticlePtr gp, HepMC3::IteratorRange range=HepMC3::ancestors) {
    if (range != HepMC3::parents && range != HepMC3::ancestors)
      throw UserError("Requested particles_in(GenParticlePtr) with a non-'in' iterator range");
    return (gp->production_vertex()) ? particles(gp->production_vertex(), range) : std::vector<GenParticlePtr>();
  }


  /// Get the direct children or all-descendents of GenParticle @a gp
  inline std::vector<GenParticlePtr> particles_out(const GenParticlePtr gp, HepMC3::IteratorRange range=HepMC3::descendants) {
    if (range != HepMC3::children && range != HepMC3::descendants)
      throw UserError("Requested particles_out(GenParticlePtr) with a non-'out' iterator range");
    return (gp->end_vertex()) ? particles(gp->end_vertex(), range) : std::vector<GenParticlePtr>();
  }


}

#endif
