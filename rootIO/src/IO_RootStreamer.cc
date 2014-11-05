/**
 *  @file IO_RootStreamer.cc
 *  @brief Implementation of \b class IO_RootStreamer
 *
 */
#include "IO_RootStreamer.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Setup.h"

#include <vector>
#include <cstring> // memset

#include <iostream>
#include <sstream>

#include "TROOT.h"
#include "TSystem.h"

#include <HepMC/foreach.h>
using std::vector;

namespace HepMC {

  IO_RootStreamer::IO_RootStreamer() {
  }

  IO_RootStreamer::IO_RootStreamer(const std::string &filename, const std::string mode) {

    TSystem ts;
    gSystem->Load("librootIO_ClassesDict");

    fFile = new TFile(filename.c_str(), mode.c_str());
    next = new TIter(fFile->GetListOfKeys());
  }

  IO_RootStreamer::~IO_RootStreamer() {
    fFile->Close();
  }



  void IO_RootStreamer::write_event(const GenEvent &evt) {

    // Clear content of m_data container
    m_data.particles.clear();
    m_data.vertices.clear();
    m_data.links1.clear();
    m_data.links2.clear();

    // Reserve memory for containers
    m_data.particles.reserve( evt.particles().size() );
    m_data.vertices.reserve( evt.vertices().size() );
    m_data.links1.reserve( evt.particles().size()*2 );
    m_data.links2.reserve( evt.particles().size()*2 );

    // Fill event data
    m_data.event_number  = evt.event_number();
    m_data.momentum_unit = evt.momentum_unit();
    m_data.length_unit   = evt.length_unit();

    // Fill containers
    FOREACH( const GenParticlePtr &p, evt.particles() ) {
      m_data.particles.push_back( p->data() );
    }

    FOREACH( const GenVertexPtr &v, evt.vertices() ) {
      m_data.vertices.push_back( v->data() );
      int v_id = v->id();

      FOREACH( const GenParticlePtr &p, v->particles_in() ) {
	m_data.links1.push_back( p->id() );
	m_data.links2.push_back( v_id    );
      }

      FOREACH( const GenParticlePtr &p, v->particles_out() ) {
	m_data.links1.push_back( v_id    );
	m_data.links2.push_back( p->id() );
      }
    }

    // Copy additional structs
    if( evt.pdf_info()      ) m_data.pdf_info      = *evt.pdf_info();
    else                      memset(&m_data.pdf_info,0,sizeof(m_data.pdf_info) );

    if( evt.heavy_ion()     ) m_data.heavy_ion     = *evt.heavy_ion();
    else                      memset(&m_data.heavy_ion,0,sizeof(m_data.heavy_ion) );

    if( evt.cross_section() ) m_data.cross_section = *evt.cross_section();
    else                      memset(&m_data.cross_section,0,sizeof(m_data.cross_section) );


    std::ostringstream os;
    os << evt.event_number();
    std::string stevt = os.str();
    const char* chevt = stevt.c_str();
    
    std::cout << "Writing event number " << stevt << std::endl;
    fFile->WriteObject(&m_data, chevt);
  }


  bool IO_RootStreamer::fill_next_event(GenEvent &evt) {

    evt.clear();
    TKey* key;

    if ((key=(TKey*)(*next)())){
      
      m_data = *((GenEventData*)key->ReadObj());
          
      // Fill event data
      evt.set_event_number( m_data.event_number );
      evt.set_units( m_data.momentum_unit, m_data.length_unit );

      // Fill particle information
      FOREACH( const GenParticleData &pd, m_data.particles ) {
        GenParticlePtr p = make_shared<GenParticle>(pd);
        evt.add_particle(p);
      }

      // Fill vertex information
      FOREACH( const GenVertexData &vd, m_data.vertices ) {
        GenVertexPtr v = make_shared<GenVertex>(vd);
        evt.add_vertex(v);
      }

      // Restore links
      for( unsigned int i=0; i<m_data.links1.size(); ++i) {
        int id1 = m_data.links1[i];
        int id2 = m_data.links2[i];

        if( id1 > 0 ) evt.vertices()[ (-id2)-1 ]->add_particle_in ( evt.particles()[ id1-1 ] );
        else          evt.vertices()[ (-id1)-1 ]->add_particle_out( evt.particles()[ id2-1 ] );
      }

      // Copy additional structs
      if( m_data.pdf_info.is_valid()      ) evt.set_pdf_info( make_shared<GenPdfInfo>(m_data.pdf_info) );
      if( m_data.heavy_ion.is_valid()     ) evt.set_heavy_ion( make_shared<GenHeavyIon>(m_data.heavy_ion) );
      if( m_data.cross_section.is_valid() ) evt.set_cross_section( make_shared<GenCrossSection>(m_data.cross_section) );

      return true;
    }
    else return false;
  } // namespace HepMC
}
