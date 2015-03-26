// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderAscii.cc
/// @brief Implementation of \b class ReaderAscii
///
#include "HepMC/ReaderAscii.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Units.h"

#include <cstring>
using namespace std;

namespace HepMC {


ReaderAscii::ReaderAscii(const string &filename)
 : m_file(filename)
{
    if( !m_file.is_open() ) {
        ERROR( "ReaderAscii: could not open input file: "<<filename )
    }
    set_run_info(make_shared<GenRunInfo>());
}


ReaderAscii::~ReaderAscii() { close(); }


bool ReaderAscii::read_event(GenEvent &evt) {
    if ( !m_file.is_open() ) return false;

    char               buf[512*512];
    bool               parsed_event_header    = false;
    bool               is_parsing_successful  = true;
    pair<int,int> vertices_and_particles(0,0);

    //
    // Parse event, vertex and particle information
    //
    while(!m_file.rdstate()) {
        m_file.getline(buf,512*512);

        if( strlen(buf) == 0 ) continue;

        // Check for ReaderAscii header/footer
        if( strncmp(buf,"HepMC",5) == 0 ) {
            if(parsed_event_header) {
                is_parsing_successful = true;
                break;
            }
            continue;
        }

        switch(buf[0]) {
            /// @todo Should consider exceptions for reporting parsing problems more locally to the source of trouble
            case 'E':
                vertices_and_particles = parse_event_information(evt,buf);
                if (vertices_and_particles.second < 0) {
                    is_parsing_successful = false;
                } else {
                    is_parsing_successful = true;
                    parsed_event_header   = true;
                }
                break;
            case 'V':
                is_parsing_successful = parse_vertex_information(evt,buf);
                break;
            case 'P':
                is_parsing_successful = parse_particle_information(evt,buf);
                break;
            case 'W':
                is_parsing_successful = parse_weight_values(evt,buf);
                break;
            case 'U':
                is_parsing_successful = parse_units(evt,buf);
                break;
            case 'A':
		if ( parsed_event_header )
		    is_parsing_successful = parse_attribute(evt,buf);
		else
		    is_parsing_successful = parse_run_attribute(buf);
                break;
            default:
                WARNING( "ReaderAscii: skipping unrecognised prefix: " << buf[0] )
                is_parsing_successful = true;
                break;
        }

        if( !is_parsing_successful ) break;

        // Check for next event
        buf[0] = m_file.peek();
        if( parsed_event_header && buf[0]=='E' ) break;
    }

    // Check if all particles and vertices were parsed
    if( (int)evt.vertices().size()  != vertices_and_particles.first ||
        (int)evt.particles().size() != vertices_and_particles.second ) {
        ERROR( "ReaderAscii: not all particles or vertices were parsed" )
        is_parsing_successful = false;
    }

    // Check if there were errors during parsing
    if( !is_parsing_successful ) {
        ERROR( "ReaderAscii: event parsing failed. Returning empty event" )
        DEBUG( 1, "Parsing failed at line:" << endl << buf )

        evt.clear();
        m_file.clear(ios::badbit);

        return false;
    }

    evt.set_run_info(run_info());

    return true;
}


pair<int,int> ReaderAscii::parse_event_information(GenEvent &evt, const char *buf) {
    static const pair<int,int>  err(-1,-1);
    pair<int,int>               ret(-1,-1);
    const char                      *cursor   = buf;
    int                              event_no = 0;

    // event number
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    event_no = atoi(cursor);
    evt.set_event_number(event_no);

    // num_vertices
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    ret.first = atoi(cursor);

    // num_particles
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    ret.second = atoi(cursor);

    DEBUG( 10, "ReaderAscii: E: "<<event_no<<" ("<<ret.first<<"V, "<<ret.second<<"P)" )

    return ret;
}


bool ReaderAscii::parse_weight_values(GenEvent &evt, const char *buf) {
    const char *cursor = buf;

    while (cursor = strchr(cursor+1,' ')) {
      ++cursor; // step past the space
      /// @todo Ick, there's no way to detect a parsing-as-float failure?!?
      const double w = atof(cursor);
      /// @todo If evt.runinfo().has_attr("WEIGHT_NAMES")... push_back(n,w)
      evt.weights().push_back(w);
    }

    return true;
}


bool ReaderAscii::parse_units(GenEvent &evt, const char *buf) {
    const char *cursor = buf;

    // momentum
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    ++cursor;
    Units::MomentumUnit momentum_unit = Units::momentum_unit(cursor);

    // length
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    ++cursor;
    Units::LengthUnit length_unit = Units::length_unit(cursor);

    evt.set_units(momentum_unit,length_unit);

    DEBUG( 10, "ReaderAscii: U: " << Units::name(evt.momentum_unit()) << " " << Units::name(evt.length_unit()) )

    return true;
}


bool ReaderAscii::parse_vertex_information(GenEvent &evt, const char *buf) {
    GenVertexPtr  data = make_shared<GenVertex>();
    FourVector    position;
    const char   *cursor          = buf;
    const char   *cursor2         = NULL;
    int           id              = 0;
    int           particle_in     = 0;
    int           highest_id      = evt.particles().size();

    // id
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    id = atoi(cursor);

    // skip to the list of particles
    if( !(cursor = strchr(cursor+1,'[')) ) return false;

    while(true) {
        ++cursor;             // skip the '[' or ',' character
        cursor2     = cursor; // save cursor position
        particle_in = atoi(cursor);

        // add incoming particle to the vertex
        if( particle_in > 0 && particle_in <= highest_id) {
            data->add_particle_in( evt.particles()[particle_in-1] );
        }
        else {
            return false;
        }

        // check for next particle or end of particle list
        if( !(cursor = strchr(cursor+1,',')) ) {
            if( !(cursor = strchr(cursor2+1,']')) ) return false;
            break;
        }
    }

    // check if there is position information
    if( (cursor = strchr(cursor+1,'@')) ) {

        // x
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setX(atof(cursor));

        // y
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setY(atof(cursor));

        // z
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setZ(atof(cursor));

        // t
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setT(atof(cursor));
        data->set_position( position );

    }

    DEBUG( 10, "ReaderAscii: V: "<<id<<" with "<<data->particles_in().size()<<" particles)" )

    evt.add_vertex(data);

    return true;
}


bool ReaderAscii::parse_particle_information(GenEvent &evt, const char *buf) {
    GenParticlePtr  data = make_shared<GenParticle>();
    FourVector      momentum;
    const char     *cursor  = buf;
    int             mother_id = 0;

    // verify id
    if( !(cursor = strchr(cursor+1,' ')) ) return false;

    if( atoi(cursor) != (int)evt.particles().size() + 1 ) {
        ERROR( "ReaderAscii: particle id mismatch" )
        return false;
    }

    // mother id
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    mother_id = atoi(cursor);

    // add particle to corresponding vertex
    if( mother_id > 0 && mother_id <= (int)evt.particles().size() ) {

        GenParticlePtr mother = evt.particles()[ mother_id-1 ];
        GenVertexPtr   vertex = mother->end_vertex();

        // create new vertex if needed
        if( !vertex ) {
            vertex = make_shared<GenVertex>();
            vertex->add_particle_in(mother);
        }

        vertex->add_particle_out(data);
        evt.add_vertex(vertex);
    }
    else if( mother_id < 0 && -mother_id <= (int)evt.vertices().size() ) {
        evt.vertices()[ (-mother_id)-1 ]->add_particle_out(data);
    }

    // pdg id
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_pdg_id( atoi(cursor) );

    // px
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPx(atof(cursor));

    // py
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPy(atof(cursor));

    // pz
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPz(atof(cursor));

    // pe
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setE(atof(cursor));
    data->set_momentum(momentum);

    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_generated_mass( atof(cursor) );

    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_status( atoi(cursor) );

    evt.add_particle(data);

    DEBUG( 10, "ReaderAscii: P: "<<data->id()<<" ( mother: "<<mother_id<<", pdg_id: "<<data->pdg_id()<<")" )

    return true;
}


bool ReaderAscii::parse_attribute(GenEvent &evt, const char *buf) {
    const char     *cursor  = buf;
    const char     *cursor2 = buf;
    char            name[64];
    int             id = 0;

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    id = atoi(cursor);

    if( !(cursor  = strchr(cursor+1,' ')) ) return false;
    ++cursor;

    if( !(cursor2 = strchr(cursor,' ')) ) return false;
    sprintf(name,"%.*s", (int)(cursor2-cursor), cursor);

    cursor = cursor2+1;

    shared_ptr<Attribute> att =
	make_shared<StringAttribute>( StringAttribute(unescape(cursor)) );

    evt.add_attribute(string(name), att, id);

    return true;
}

bool ReaderAscii::parse_run_attribute(const char *buf) {
    const char     *cursor  = buf;
    const char     *cursor2 = buf;
    char            name[64];

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    ++cursor;

    if( !(cursor2 = strchr(cursor,' ')) ) return false;
    sprintf(name,"%.*s", (int)(cursor2-cursor), cursor);

    cursor = cursor2+1;

     shared_ptr<StringAttribute> att =
	 make_shared<StringAttribute>( StringAttribute(unescape(cursor)) );

    run_info()->add_attribute(string(name), att);

    return true;

}


string ReaderAscii::unescape(const string s) {
    string ret;
    ret.reserve(s.length());
    for ( string::const_iterator it = s.begin(); it != s.end(); ++it ) {
        if ( *it == '\\' ) {
            ++it;
            if ( *it == '|' )
                ret += '\n';
            else
                ret += *it;
        } else
            ret += *it;
    }

    return ret;
}


void ReaderAscii::close() {
    if( !m_file.is_open() ) return;
    m_file.close();
}


} // namespace HepMC
