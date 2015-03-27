// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class GenEvent
 *
 */
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"

#include "HepMC/Data/GenEventData.h"
#include "HepMC/Search/FindParticles.h"

#include <deque>
using namespace std;

namespace HepMC {


GenEvent::GenEvent(Units::MomentumUnit momentum_unit,
		   Units::LengthUnit length_unit)
  : m_event_number(0), m_momentum_unit(momentum_unit),
    m_length_unit(length_unit),
    m_rootvertex(make_shared<GenVertex>()) {}


GenEvent::GenEvent(shared_ptr<GenRunInfo> run,
	 Units::MomentumUnit momentum_unit,
	 Units::LengthUnit length_unit)
  : m_event_number(0), m_momentum_unit(momentum_unit),
    m_length_unit(length_unit),
    m_rootvertex(make_shared<GenVertex>()),
    m_run_info(run) {}


// void GenEvent::add_particle( const GenParticlePtr &p ) {
void GenEvent::add_particle( GenParticlePtr p ) {
    if( p->in_event() ) return;

    m_particles.push_back(p);

    p->m_event = this;
    p->m_id    = particles().size();

    // Particles without production vertex are added to the root vertex
    if( !p->production_vertex() )
      m_rootvertex->add_particle_out(p);
}


// void GenEvent::add_vertex( const GenVertexPtr &v ) {
void GenEvent::add_vertex( GenVertexPtr v ) {
    if( v->in_event() ) return;

    m_vertices.push_back(v);

    v->m_event = this;
    v->m_id    = -(int)vertices().size();

    // Add all incoming and outgoing particles and restore their production/end vertices
    FOREACH( GenParticlePtr &p, v->m_particles_in ) {
        if(!p->in_event()) add_particle(p);
        p->m_end_vertex = v->m_this.lock();
    }

    FOREACH( GenParticlePtr &p, v->m_particles_out ) {
        if(!p->in_event()) add_particle(p);
        p->m_production_vertex = v->m_this.lock();
    }
}


void GenEvent::remove_particle( GenParticlePtr p ) {
    if( !p || p->parent_event() != this ) return;

    DEBUG( 3, "GenEvent::remove_particle - called with particle: "<<p->id() );
    GenVertexPtr end_vtx = p->end_vertex();
    if( end_vtx ) {
        end_vtx->remove_particle_in(p);

        // If that was the only incoming particle, remove vertex from the event
        if( end_vtx->particles_in().size() == 0 )  remove_vertex(end_vtx);
    }

    GenVertexPtr prod_vtx = p->production_vertex();
    if( prod_vtx ) {
        prod_vtx->remove_particle_out(p);

        // If that was the only outgoing particle, remove vertex from the event
        if( prod_vtx->particles_out().size() == 0 ) remove_vertex(prod_vtx);
    }

    DEBUG( 3,"GenEvent::remove_particle - erasing particle: " << p->id() )

    int idx = p->id();
    vector<GenParticlePtr>::iterator it = m_particles.erase(m_particles.begin() + idx-1 );

    // Reassign id of the particles following this one
    for(;it != m_particles.end(); ++it) {
        (*it)->m_id = idx;
        ++idx;
    }
}

void GenEvent::remove_vertex( GenVertexPtr v ) {
    if( !v || v->parent_event() != this ) return;

    DEBUG( 3, "GenEvent::remove_vertex   - called with vertex:  "<<v->id() );
    shared_ptr<GenVertex> null_vtx;

    FOREACH( GenParticlePtr &p, v->m_particles_in ) {
        p->m_end_vertex.reset();
    }

    FOREACH( GenParticlePtr &p, v->m_particles_out ) {
        p->m_production_vertex.reset();

        // recursive delete rest of the tree
        remove_particle(p);
    }

    // Erase this vertex from vertices list
    DEBUG( 3, "GenEvent::remove_vertex   - erasing vertex: " << v->id() )

    int idx = -v->id();
    vector<GenVertexPtr>::iterator it = m_vertices.erase(m_vertices.begin() + idx-1 );

    // Reassign id of the vertices following this one
    for(;it != m_vertices.end(); ++it) {
        (*it)->m_id = -idx;
        ++idx;
    }
}


void GenEvent::add_tree( const vector<GenParticlePtr> &particles ) {

    deque<GenVertexPtr> sorting;

    // Find all starting vertices (end vertex of particles that have no production vertex)
    FOREACH( const GenParticlePtr &p, particles ) {
        const GenVertexPtr &v = p->production_vertex();
        if( !v || v->particles_in().size()==0 ) {
            const GenVertexPtr &v2 = p->end_vertex();
            if(v2) sorting.push_back(v2);
        }
    }

    DEBUG_CODE_BLOCK(
        unsigned int sorting_loop_count = 0;
        unsigned int max_deque_size     = 0;
    )

    // Add vertices to the event in topological order
    while( !sorting.empty() ) {
        DEBUG_CODE_BLOCK(
            if( sorting.size() > max_deque_size ) max_deque_size = sorting.size();
            ++sorting_loop_count;
        )

        GenVertexPtr &v = sorting.front();

        bool added = false;

        // Add all mothers to the front of the list
        FOREACH( const GenParticlePtr &p, v->particles_in() ) {
            GenVertexPtr v2 = p->production_vertex();
            if( v2 && !v2->in_event() ) {
                sorting.push_front(v2);
                added = true;
            }
        }

        // If we have added at least one production vertex,
        // our vertex is not the first one on the list
        if( added ) continue;

        // If vertex not yet added
        if( !v->in_event() ) {

            add_vertex(v);

            // Add all end vertices to the end of the list
            FOREACH( const GenParticlePtr &p, v->particles_out() ) {
                GenVertexPtr v2 = p->end_vertex();
                if( v2 && !v2->in_event() ) {
                    sorting.push_back(v2);
                }
            }
        }

        sorting.pop_front();
    }

    DEBUG_CODE_BLOCK(
        DEBUG( 6, "GenEvent - particles sorted: "
                   <<this->particles().size()<<", max deque size: "
                   <<max_deque_size<<", iterations: "<<sorting_loop_count )
    )
}


void GenEvent::reserve(unsigned int particles, unsigned int vertices) {
    m_particles.reserve(particles);
    m_vertices.reserve(vertices);
}


void GenEvent::set_units( Units::MomentumUnit new_momentum_unit, Units::LengthUnit new_length_unit) {
    if( new_momentum_unit != m_momentum_unit ) {
        FOREACH( GenParticlePtr &p, m_particles ) {
            Units::convert( p->m_data.momentum, m_momentum_unit, new_momentum_unit );
        }

        m_momentum_unit = new_momentum_unit;
    }

    if( new_length_unit != m_length_unit ) {
        FOREACH( GenVertexPtr &v, m_vertices ) {
            FourVector &fv = v->m_data.position;
            if( !fv.is_zero() ) Units::convert( fv, m_length_unit, new_length_unit );
        }

        m_length_unit = new_length_unit;
    }
}


const FourVector& GenEvent::event_pos() const {
  return m_rootvertex->position();
}


void GenEvent::shift_position_by( const FourVector & delta ) {
    m_rootvertex->set_position( event_pos() + delta );

    // Offset all vertices
    FOREACH ( GenVertexPtr &v, m_vertices ) {
        if ( v->has_set_position() )
            v->set_position( v->position() + delta );
    }
}


void GenEvent::clear() {
    m_event_number = 0;
    m_attributes.clear();
    m_particles.clear();
    m_vertices.clear();
}


//
// Deprecated functions
//

void GenEvent::add_particle( GenParticle *p ) {
    add_particle( GenParticlePtr(p) );
}


void GenEvent::add_vertex( GenVertex *v ) {
    add_vertex( GenVertexPtr(v) );
}


void GenEvent::remove_attribute(const string &name, int id) {
    map< string, map<int, shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if( i1 == m_attributes.end() ) return;

    map<int, shared_ptr<Attribute> >::iterator i2 = i1->second.find(id);
    if( i2 == i1->second.end() ) return;

    i1->second.erase(i2);
}


void GenEvent::write_data(GenEventData& data) const {
    // Reserve memory for containers
    data.particles.reserve( this->particles().size() );
    data.vertices.reserve( this->vertices().size() );
    data.links1.reserve( this->particles().size()*2 );
    data.links2.reserve( this->particles().size()*2 );
    data.attribute_id.reserve( this->attributes().size() );
    data.attribute_name.reserve( this->attributes().size() );
    data.attribute_string.reserve( this->attributes().size() );

    // Fill event data
    data.event_number  = this->event_number();
    data.momentum_unit = this->momentum_unit();
    data.length_unit   = this->length_unit();

    // Fill containers
    FOREACH( const GenParticlePtr &p, this->particles() ) {
        data.particles.push_back( p->data() );
    }

    FOREACH( const GenVertexPtr &v, this->vertices() ) {
        data.vertices.push_back( v->data() );
        int v_id = v->id();

        FOREACH( const GenParticlePtr &p, v->particles_in() ) {
            data.links1.push_back( p->id() );
            data.links2.push_back( v_id    );
        }

        FOREACH( const GenParticlePtr &p, v->particles_out() ) {
            data.links1.push_back( v_id    );
            data.links2.push_back( p->id() );
        }
    }

    typedef map< string, map<int, shared_ptr<Attribute> > >::value_type value_type1;
    typedef map<int, shared_ptr<Attribute> >::value_type                value_type2;

    FOREACH( const value_type1& vt1, this->attributes() ) {
        FOREACH( const value_type2& vt2, vt1.second ) {

            string st;

            bool status = vt2.second->to_string(st);

            if( !status ) {
                WARNING( "GenEvent::write_data: problem serializing attribute: "<<vt1.first )
            }
            else {
                data.attribute_id.push_back(vt2.first);
                data.attribute_name.push_back(vt1.first);
                data.attribute_string.push_back(st);
            }
        }
    }
}


void GenEvent::read_data(const GenEventData &data) {
    this->set_event_number( data.event_number );
    this->set_units( data.momentum_unit, data.length_unit );

    // Fill particle information
    FOREACH( const GenParticleData &pd, data.particles ) {
      GenParticlePtr p = make_shared<GenParticle>(pd);
      this->add_particle(p);
    }

    // Fill vertex information
    FOREACH( const GenVertexData &vd, data.vertices ) {
      GenVertexPtr v = make_shared<GenVertex>(vd);
        this->add_vertex(v);
    }

    // Restore links
    for( unsigned int i=0; i<data.links1.size(); ++i) {
      int id1 = data.links1[i];
      int id2 = data.links2[i];

      if( id1 > 0 ) m_vertices[ (-id2)-1 ]->add_particle_in ( m_particles[ id1-1 ] );
      else          m_vertices[ (-id1)-1 ]->add_particle_out( m_particles[ id2-1 ] );
    }

    // Read attributes
    for( unsigned int i=0; i<data.attribute_id.size(); ++i) {
        add_attribute( data.attribute_name[i],
                       make_shared<StringAttribute>(data.attribute_string[i]),
                       data.attribute_id[i] );
    }
}


#ifndef HEPMC_NO_DEPRECATED

bool GenEvent::valid_beam_particles() const {
    /// @todo Change this definition to require status = 4... and in principle there don't have to be two of them
    return (m_rootvertex->particles_out().size()==2);
}

pair<GenParticlePtr,GenParticlePtr> GenEvent::beam_particles() const {
    /// @todo Change this definition to require status = 4... and in principle there don't have to be two of them
    switch( m_rootvertex->particles_out().size() ) {
        case 0:  return make_pair(GenParticlePtr(),              GenParticlePtr());
        case 1:  return make_pair(m_rootvertex->particles_out()[0], GenParticlePtr());
        default: return make_pair(m_rootvertex->particles_out()[0], m_rootvertex->particles_out()[1]);
    }
}

void GenEvent::set_beam_particles(const GenParticlePtr& p1, const GenParticlePtr& p2) {
    /// @todo Require/set status = 4
    m_rootvertex->add_particle_out(p1);
    m_rootvertex->add_particle_out(p2);
}

void GenEvent::set_beam_particles(const pair<GenParticlePtr,GenParticlePtr>& p) {
    /// @todo Require/set status = 4
    m_rootvertex->add_particle_out(p.first);
    m_rootvertex->add_particle_out(p.second);
}

#endif

string GenEvent::attribute_as_string(const string &name, int id) const {

    std::map< string, std::map<int, shared_ptr<Attribute> > >::iterator i1 = m_attributes.find(name);
    if( i1 == m_attributes.end() ) {
        if ( id == 0 && run_info() ) {
            return run_info()->attribute_as_string(name);
        }
        return string();
    }

    std::map<int, shared_ptr<Attribute> >::iterator i2 = i1->second.find(id);
    if (i2 == i1->second.end() ) return string();

    if( !i2->second ) return string();

    string ret;
    i2->second->to_string(ret);

    return ret;
}

#ifdef HEPMC_ROOTIO

void GenEvent::Streamer(TBuffer &b){

  if (b.IsReading()){

    GenEventData data;

    b.ReadClassBuffer(TClass::GetClass("HepMC::GenEventData"), &data);

    read_data(data);

  } else {

    // fill the GenEventData structures
    GenEventData data;
    write_data(data);

    b.WriteClassBuffer(TClass::GetClass("HepMC::GenEventData"), &data);
  }
}

#endif


} // namespace HepMC
