// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR.cc
/// @brief Implementation of \b class ReaderOSCAR
///
#include <cstring>
#include <sstream>

#include "HepMC3/ReaderOSCAR.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"

namespace HepMC3 {


ReaderOSCAR::ReaderOSCAR(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR::ReaderOSCAR(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}



ReaderOSCAR::~ReaderOSCAR() {
    if (!m_isstream) close();
}

bool ReaderOSCAR::skip(const int n)
{

    return true;
}


bool ReaderOSCAR::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    m_vertices.clear();
    m_prod.clear();
    m_prod2.clear();
    evt.set_run_info(run_info());

    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 ) continue;
        if ( strncmp(buf, "\r", 1) == 0 ) continue;
        // Check for ReaderOSCAR header/footer
        if ( strncmp(buf, "#", 1) == 0 ) {
            if ( strncmp(buf, "# OSC1999A", 10) == 0 ) {
                HEPMC3_WARNING("ReaderOSCAR: So far OSCAR format is not fully supported. ")
                m_header.clear();
            } else {
                m_header.push_back(std::string(buf));
            }
            continue;
        }
        if ( strncmp(buf, "0 0 ", 4) == 0 ) {
            cursor = strchr(cursor+3, ' ');
            evt.set_event_number(atoi(cursor));
            cursor = strchr(cursor+1, ' ');
            double impact_parameter = atof(cursor);
            evt.add_attribute("impact_parameter", std::make_shared<DoubleAttribute>(impact_parameter));
            end_event();
            evt.set_run_info(run_info());
            if (m_vertices.empty()) continue;
            for ( auto v: m_vertices) {
                evt.add_vertex(v);
            }

            for (auto& pp: m_prod)
            {
                auto& history = pp.second;
                std::sort(std::begin(history),
                          std::end(history),
                [](std::pair<GenParticlePtr,FourVector> a, std::pair<GenParticlePtr,FourVector> b) {
                    if (a.first->end_vertex() == b.first->production_vertex() && a.first->end_vertex() ) return false;
                    if (b.first->end_vertex() == a.first->production_vertex() && b.first->end_vertex() ) return true;
                    return a.second.t() > b.second.t();
                });
            }
            for (auto& pp: m_prod) {
                auto& history = pp.second;
                bool  stablep = (history.front().first->end_vertex()==nullptr);
                if (stablep )history.front().first->set_status(1);
                for (int i=1*stablep; i<history.size()-1; i+=2) {
                    auto t= history.at(i).first;
                    auto n= history.at(i+1).first;
                    if (n->production_vertex()&&!n->end_vertex()) {
                        auto v= t->end_vertex();
                        v->remove_particle_in(t);
                        evt.remove_particle(t);
                        v->add_particle_in(n);
                    }
                    if (!n->production_vertex())
                    {
                        n->set_status(4);
                        evt.add_beam_particle(n);
                    }
                }
                auto sp= history.back().first;
                if (sp->production_vertex()&&sp->production_vertex()->id()==0) {
                    sp->set_status(4);
                    evt.add_beam_particle(sp);
                }
            }

            evt.weights() = std::vector<double>(1,1);
            break;
        }
        int nin = atoi(cursor);
        cursor = strchr(cursor+1, ' ');
        int nout = atoi(cursor);
        m_interaction.clear();
        m_interaction.push_back(std::string(buf));
        for (int i=0; i<nin+nout; i++ ) {
            m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
            m_interaction.push_back(std::string(buf));
        }
        parse_interaction();
    }
    return true;
}


GenParticlePtr ReaderOSCAR::parse_particle(const std::string& s) {
    char buf[512];
    sprintf(buf,"%s\n",s.c_str());
    const char  *cursor   = buf;
    int id = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    int pdg = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double px= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double py= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double pz= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double p0= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double m= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double x= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double y= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double z= atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double t= atof(cursor);
    GenParticlePtr p = std::make_shared<GenParticle>(FourVector(px,py,pz,p0),pdg,3);
    p->set_generated_mass(m);
    m_prod[id].push_back(std::pair<GenParticlePtr,FourVector>(p,FourVector(x,y,z,t)));
    m_prod2[p] = std::pair<int,FourVector>(id,FourVector(x,y,z,t));
    return p;
}

void ReaderOSCAR::parse_interaction() {
    GenVertexPtr v = std::make_shared<GenVertex>();
    char buf[512];
    sprintf(buf,"%s\n",m_interaction.at(0).c_str());
    const char  *cursor   = buf;
    int nin = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return;
    int nout = atoi(cursor);
    for (int i=1; i<nin+1; i++ ) v->add_particle_in(parse_particle(m_interaction.at(i)));
    for (int i=nin+1; i<nout+nin+1; i++ ) v->add_particle_out(parse_particle(m_interaction.at(i)));
    m_vertices.push_back(v);
    std::vector< std::pair<int,FourVector> > vin;
    for (auto p: v->particles_out()) vin.push_back(m_prod2[p]);
    std::sort(std::begin(vin),
              std::end(vin),
    [](std::pair<int,FourVector> a, std::pair<int,FourVector> b) {
        return a.second.t() > b.second.t();
    });
    v->set_position(vin.front().second);
}
void ReaderOSCAR::end_event() {
    parse_header();


}
void ReaderOSCAR::parse_header() {
    if (m_header.empty()) return;
    for (auto &s: m_header) s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
    if (m_header.size() > 2 && m_header[1].length() > 2) {
        struct GenRunInfo::ToolInfo generator = {m_header[1].substr(2), "0", std::string("Used generator")};
        auto ri = run_info();
        ri->tools().push_back(generator);
        std::vector<std::string> weightnames;
        weightnames.push_back("Default");
        ri->set_weight_names(weightnames);
        set_run_info(ri);
    }
}

bool ReaderOSCAR::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}


} // namespace HepMC3
