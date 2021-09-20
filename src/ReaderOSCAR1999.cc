// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR1999.cc
/// @brief Implementation of \b class ReaderOSCAR1999
///
#include <cstring>
#include <sstream>
#include <cassert>

#include "HepMC3/ReaderOSCAR1999.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"

namespace HepMC3 {


ReaderOSCAR1999::ReaderOSCAR1999(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR1999: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1999::ReaderOSCAR1999(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR1999: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1999::~ReaderOSCAR1999() {
    if (!m_isstream) close();
}

bool ReaderOSCAR1999::skip(const int n)
{
    return true;
}

inline void filter_spaces(char* actual_input) {
    const char* input = actual_input;
    char* output = actual_input;
    ssize_t j = 0;
    ssize_t l = strlen(input);
    if (!l) return;
    for (ssize_t i = 1; i < l; i++) {
        if ( !std::isspace(input[i]) || !std::isspace(output[j]))
        { j++; output[j] = input[i]; }
    }
    j++;
    output[j] = '\0';
}

bool ReaderOSCAR1999::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    m_vertices.clear();
    m_prod.clear();
    m_prod2.clear();
    evt.set_units(Units::GEV,Units::MM);//Fixed units?
    evt.set_run_info(run_info());
    bool footer = false;


    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 && footer) continue;
        if ( strncmp(buf, "\r", 1) == 0 ) continue;
        // Check for ReaderOSCAR1999 header/footer
        if ( strncmp(buf, "#", 1) == 0 ) {
            if ( strncmp(buf, "# OSC1999A", 10) == 0 ) {
                HEPMC3_WARNING("ReaderOSCAR1999: So far OSCAR1999A format is not fully supported. ")
                m_header.clear();
            } else  {
                if  ( strncmp(buf, "# End of event:", 15) == 0 ) footer =  true;
                m_header.push_back(std::string(buf));

            }
            continue;
        }
        if (  (strlen(buf) != 0 && ( strncmp(buf, "0 0 ", 4) == 0 ||strncmp(buf, "      0        0", 16) == 0) ) || ( strlen(buf) == 0 && !footer) ) {

            if (footer && strlen(buf) != 0 ) {
                while (cursor && *cursor == ' ') cursor++;
                cursor = strchr(cursor+1, ' ');
                evt.set_event_number(atoi(cursor));
                while (cursor && *cursor == ' ') cursor++;
                cursor = strchr(cursor+1, ' ');
                double impact_parameter = atof(cursor);
                evt.add_attribute("impact_parameter", std::make_shared<DoubleAttribute>(impact_parameter));
            }
            end_event();
            evt.set_run_info(run_info());
            std::vector<GenParticlePtr> toremove;
            for ( auto vvv: m_vertices) {
                if (vvv->particles_in().empty()) {
                    for (auto ppp: vvv->particles_out()) {
                        toremove.push_back(ppp);
                    }
                }
                else evt.add_vertex(vvv);
            }
            for (auto ppp: toremove) {
                if (ppp->production_vertex()) ppp->production_vertex()->remove_particle_out(ppp);
                evt.add_particle(ppp);
                evt.add_beam_particle(ppp);

            }

            for (auto& pp: m_prod)
            {
                auto& history = pp.second;
                std::sort(std::begin(history), std::end(history),
                [](const std::pair<GenParticlePtr,FourVector>& a, const std::pair<GenParticlePtr,FourVector>& b) {
                    if (a.first->end_vertex() == b.first->production_vertex() && a.first->end_vertex() ) return false;
                    if (b.first->end_vertex() == a.first->production_vertex() && b.first->end_vertex() ) return true;
                    if (a.first->production_vertex() &&  !b.first->production_vertex() ) return true;
                    if (b.first->production_vertex() &&  !a.first->production_vertex() ) return false;
                    return a.second.t() > b.second.t();
                });
            }
            for (auto& pp: m_prod) {
                auto& history = pp.second;
                bool  stablep = (history.front().first->end_vertex() == nullptr);
                if (stablep) history.front().first->set_status(1);
                for (int i = 1*stablep; i<history.size()-1; i += 2 ) {
                    auto t = history.at(i).first;
                    auto n = history.at(i+1).first;
                    if (n->production_vertex()&&!n->end_vertex()) {
                        auto v = t->end_vertex();

                        evt.remove_particle(t);
                        if (v) {
                            v->remove_particle_in(t);
                            v->add_particle_in(n);
                        }
                    }
                    if (!n->production_vertex())
                    {
                        n->set_status(4);
                        evt.add_beam_particle(n);
                    }
                }
                auto sp = history.back().first;
                if ( sp->production_vertex() && sp->production_vertex()->id() == 0) {
                    sp->set_status(4);
                    evt.add_beam_particle(sp);
                }
            }

            evt.weights() = std::vector<double>(1,1);
            break;
        }
        if ( strlen(buf) == 0 ) continue;
        cursor = buf;
        while (cursor && *cursor == ' ') cursor++;
        int nin = atoi(cursor);
        cursor = strchr(cursor+1, ' ');
        int nout = atoi(cursor);
        std::vector<std::string> interaction; ///< header lines
        interaction.push_back(std::string(buf));
        for (int i = 0; i< nin + nout; i++ ) {
            m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
            filter_spaces(buf);
            interaction.push_back(std::string(buf));
        }
        parse_interaction(evt,interaction);
    }
    return true;
}

GenParticlePtr ReaderOSCAR1999::parse_particle(const std::string& s) {
    char buf[512];
    sprintf(buf, "%s\n", s.c_str());
    const char  *cursor = buf;
    while (cursor && *cursor == ' ') cursor++;
    int id = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    int pdg = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    int wft = atoi(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double px = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double py = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double pz = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double p0 = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double m = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double x = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double y = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double z = atof(cursor);
    if ( !(cursor = strchr(cursor+1, ' ')) ) return nullptr;
    double t = atof(cursor);
    GenParticlePtr p = std::make_shared<GenParticle>(FourVector(px,py,pz,p0),pdg,3);
    p->set_generated_mass(m);
    m_prod[id].push_back(std::pair<GenParticlePtr,FourVector>(p,FourVector(x,y,z,t)));
    m_prod2[p] = std::pair<int,FourVector>(id,FourVector(x,y,z,t));
    return p;
}

void ReaderOSCAR1999::parse_interaction(GenEvent &evt,const std::vector<std::string>& interaction) {
    GenVertexPtr v = std::make_shared<GenVertex>();
    char buf[512];
    sprintf(buf, "%s\n", interaction.at(0).c_str());
    const char  *cursor   = buf;
    while (cursor && *cursor == ' ') cursor++;
    int nin = atoi(cursor);
    if ( !(cursor = strchr(cursor + 1, ' ')) ) return;
    int nout = atoi(cursor);
    cursor = strchr(cursor + 1, ' ');
    double density = atof(cursor);
    cursor = strchr(cursor + 1, ' ');
    double tot_weight = atof(cursor);
    cursor = strchr(cursor + 1, ' ');
    double part_weight = atof(cursor);
    cursor = strchr(cursor + 1, ' ');
    int proc_type = atoi(cursor);

    for (int i = 1; i < nin + 1; i++ ) v->add_particle_in(parse_particle(interaction.at(i)));
    assert(nout + nin + 1 == interaction.size());
    for (int i = nin + 1; i < nout + nin + 1; i++ ) v->add_particle_out(parse_particle(interaction.at(i)));
    m_vertices.push_back(v);
    std::vector< std::pair<int,FourVector> > vin;
    for (auto p: v->particles_out()) vin.push_back(m_prod2.at(p));
    std::sort(std::begin(vin), std::end(vin),
    [](std::pair<int,FourVector> a, std::pair<int,FourVector> b) {
        return a.second.t() > b.second.t();
    });
    if (!vin.empty()) v->set_position(vin.front().second);
    evt.add_vertex(v);
    v->add_attribute("density", std::make_shared<DoubleAttribute>(density));
    v->add_attribute("tot_weight", std::make_shared<DoubleAttribute>(tot_weight));
    v->add_attribute("part_weight", std::make_shared<DoubleAttribute>(part_weight));
    v->add_attribute("proc_type", std::make_shared<IntAttribute>(proc_type));
}

void ReaderOSCAR1999::end_event() {
    parse_header();
}

void ReaderOSCAR1999::parse_header() {
    if (m_header.empty()) return;
    for (auto &s: m_header) s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
    if (m_header.size() > 2&& m_header.at(0).length() > 2 && m_header.at(1).length() > 2) {
        run_info()->add_attribute("content", std::make_shared<StringAttribute>(m_header.at(0).substr(2)));

        std::string toparse1 = m_header.at(1);
        toparse1[0]=' ';
        std::vector<std::string> parsed1;
        while (toparse1.size()>0)
        {
            toparse1.erase(0,toparse1.find_first_not_of(" -"));
            parsed1.push_back(toparse1.substr(0,toparse1.find_first_of(" -")));
            toparse1.erase(0,parsed1.back().size());
        }

        struct GenRunInfo::ToolInfo generator = {parsed1.size()>1?parsed1.at(0):"Unknown", parsed1.size()>2?parsed1.at(1):"0.0.0", std::string("Used generator")};
        run_info()->tools().push_back(generator);

        std::string toparse2 = m_header.at(2);
        toparse2[0]=' ';
        size_t reaction_b = toparse2.find_first_of("(");
        size_t reaction_e = toparse2.find_last_of(")")+1;


        if (reaction_e > reaction_b + 1) {
            std::string reaction = toparse2.substr(reaction_b,reaction_e-reaction_b);
            toparse2.erase(reaction_b,reaction_e-reaction_b);
            std::vector<std::string> parsed2;
            while (toparse2.size()>0)
            {
                toparse2.erase(0,toparse2.find_first_not_of(" "));
                parsed2.push_back(toparse2.substr(0,toparse2.find_first_of(" ")));
                toparse2.erase(0,parsed2.back().size());
            }
            run_info()->add_attribute("reaction", std::make_shared<StringAttribute>(reaction));
            if (parsed2.size() > 0) run_info()->add_attribute("reference_frame", std::make_shared<StringAttribute>(parsed2.at(0)));
            if (parsed2.size() > 1) run_info()->add_attribute("beam_energy", std::make_shared<DoubleAttribute>(std::strtof(parsed2.at(1).c_str(),NULL)));
            if (parsed2.size() > 2) run_info()->add_attribute("test_particles_per_nuclon", std::make_shared<IntAttribute>(std::atoi(parsed2.at(2).c_str())));
        }

        std::vector<std::string> weightnames;
        weightnames.push_back("Default");
        run_info()->set_weight_names(weightnames);
    }
}

bool ReaderOSCAR1999::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR1999::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
