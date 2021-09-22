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
inline std::vector<std::string> tokenize_string(const std::string& str, const std::string& delimiters )
{
    std::vector<std::string> tokens;
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
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
        if (  (strlen(buf) != 0 && ( strncmp(buf, "0 0 ", 4) == 0 ||strncmp(buf, "      0        0", 16) == 0) ) ) {
            std::string st(buf);
            std::vector<std::string> endline = tokenize_string(st, " ");
            parse_header();
            // This should be the end of event
            if (endline.size() > 1) {
                if (endline.size() > 2) evt.set_event_number(atoi(endline.at(2).c_str()));
                if (endline.size() > 3)
                {
                    GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
                    hi->set(-1, -1, -1, -1, -1, -1,
                            -1, -1, -1,
                            -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
                    hi->impact_parameter = std::strtof(endline.at(3).c_str(),nullptr);
                    evt.set_heavy_ion(hi);
                }
            }
            auto cs = std::make_shared<GenCrossSection>();
            cs->set_cross_section(1.0,1.0);
            evt.set_cross_section(cs);
            evt.weights() = std::vector<double>(1,1);
            evt.set_run_info(run_info());

            for (auto& pp: m_prod)
            {
                auto& history = pp.second;
                if (history.size() == 1) {
                    if (!history.at(0).first->production_vertex()) evt.add_beam_particle(history.at(0).first);
                }
                for (int i = 0; i < history.size()-1; i += 2 )
                {
                    auto t = history.at(i).first;
                    auto n = history.at(i+1).first;
                    auto pv_t = t->production_vertex();
                    auto ev_t = t->end_vertex();
                    auto pv_n = n->production_vertex();
                    auto ev_n = n->end_vertex();
                    if (pv_t && !ev_t  && ev_n)
                    {
                        ev_n->add_particle_in(t);
                        ev_n->remove_particle_in(n);
                        evt.remove_particle(n);
                    }
                }
            }
            std::vector<GenVertexPtr> vtoremove;
            for (auto v: evt.vertices()) if (!v->particles_in().size() || !v->particles_out().size()) vtoremove.push_back(v);
            for (auto v: vtoremove) {
                evt.remove_vertex(v);
            }
            std::vector<GenParticlePtr> ptoremove;
            for (auto p: evt.particles()) if (!p->production_vertex() && !p->end_vertex() && p->status() != 4) ptoremove.push_back(p);
            for (auto p: ptoremove) evt.remove_particle(p);
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
        for (int i = 0; i < nin + nout; i++ ) {
            m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
            if ( strlen(buf) == 0 ) {
                HEPMC3_ERROR("ReaderOSCAR1999: event parsing failed. Returning empty event")
                HEPMC3_DEBUG(1, "Parsing failed after line:" << std::endl << interaction.back().c_str())
            }
            interaction.push_back(std::string(buf));
        }
        parse_interaction(evt,interaction);
    }
    return true;
}

GenParticlePtr ReaderOSCAR1999::parse_particle(const std::string& s) {
    std::vector<std::string> particle_tokens = tokenize_string(s, " ");
    int id = atoi(particle_tokens.at(0).c_str());
    int pdg = atoi(particle_tokens.at(1).c_str());
    int wft = atoi(particle_tokens.at(2).c_str());
    double px = atof(particle_tokens.at(3).c_str());
    double py = atof(particle_tokens.at(4).c_str());
    double pz = atof(particle_tokens.at(5).c_str());
    double p0 = atof(particle_tokens.at(6).c_str());
    double m = atof(particle_tokens.at(7).c_str());
    double x = atof(particle_tokens.at(8).c_str());
    double y = atof(particle_tokens.at(9).c_str());
    double z = atof(particle_tokens.at(10).c_str());
    double t = atof(particle_tokens.at(11).c_str());
    GenParticlePtr p = std::make_shared<GenParticle>(FourVector(px,py,pz,p0),pdg,3);
    p->set_generated_mass(m);
    m_prod[id].push_back(std::pair<GenParticlePtr,FourVector>(p,FourVector(x,y,z,t)));
    m_prod2[p] = std::pair<int,FourVector>(id,FourVector(x,y,z,t));
    return p;
}

void ReaderOSCAR1999::parse_interaction(GenEvent &evt,const std::vector<std::string>& interaction) {

//OSCAR1999A is implemented inconsistently across the generators.
//UrqMD has three versions, the input the interaction and the output
//->      0       24        1     0.971     0.000<-
// UrQd amnual page 33
//->          2        2        1        1   1.577  0.2611E+01  0.4212E+02  0.1035E+02  0.1221E+00<-
// UrQd amnual page 27
//->      32        0<-
// Not documented
//SMASH has only interaction format
//->2 2    0.0000000   45.2577226   21.0598335     1<-
    std::vector<std::string> interaction_header_tokens = tokenize_string(interaction.at(0), " ");
    int nin = atoi(interaction_header_tokens.at(0).c_str());
    int nout = atoi(interaction_header_tokens.at(1).c_str());
    printf("%i %i\n",nin,nout);
    GenVertexPtr v = std::make_shared<GenVertex>();
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
    if (nin == 0 && nout != 0 ) {
        auto particles_out = v->particles_out();
        for (auto p: particles_out)
        {
            v->remove_particle_out(p);
            p->set_status(4);
            evt.add_beam_particle(p);
        }
        if (interaction_header_tokens.size() == 5 )
        {
            evt.set_event_number(atoi(interaction_header_tokens.at(2).c_str()));
            GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
            hi->set(-1, -1, -1, -1, -1, -1,
                    -1, -1, -1,
                    -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
            hi->impact_parameter = std::strtof(interaction_header_tokens.at(3).c_str(),nullptr);
            hi->event_plane_angle = std::strtof(interaction_header_tokens.at(4).c_str(),nullptr); //NOT DOCUMENTED IN MANUALS!!!
            evt.set_heavy_ion(hi);
        }
    }

    if (nin != 0 && nout == 0 ) {
        auto particles_final = v->particles_in();
        for (auto p: particles_final)
        {
            v->remove_particle_in(p);
            p->set_status(1);
            evt.add_particle(p);
        }
    }

    if (nin !=0 && nout !=0 )
    {
        evt.add_vertex(v);
        if (interaction_header_tokens.size() == 9 )
        {
            v->add_attribute("process_type", std::make_shared<IntAttribute>(atoi(interaction_header_tokens.at(2).c_str())));
            //v->add_attribute("collision_counter", std::make_shared<IntAttribute>(std::strtol(tokens.at(3))));//
            v->add_attribute("collision_time", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(4).c_str(),nullptr)));
            v->add_attribute("cms", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(5).c_str(),nullptr)));
            v->add_attribute("total_cross_section", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(6).c_str(),nullptr)));
            v->add_attribute("partial_cross_section", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(7).c_str(),nullptr)));
            v->add_attribute("baryon_density", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(8).c_str(),nullptr)));
        }
        if (interaction_header_tokens.size() == 6 )
        {
            v->add_attribute("baryon_density", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(2).c_str(),nullptr)));
            v->add_attribute("total_weight", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(3).c_str(),nullptr)));
            v->add_attribute("partial_weight", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(4).c_str(),nullptr)));
            v->add_attribute("process_type", std::make_shared<IntAttribute>(atoi(interaction_header_tokens.at(5).c_str())));
        }
    }
}

void ReaderOSCAR1999::parse_header() {
    if (m_header.empty()) return;
    for (auto &s: m_header) s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
    if (m_header.size() > 2&& m_header.at(0).length() > 2 && m_header.at(1).length() > 2) {
        run_info()->add_attribute("content", std::make_shared<StringAttribute>(m_header.at(0).substr(2)));
        std::string toparse1 = m_header.at(1);
        std::vector<std::string> parsed1 = tokenize_string(toparse1, " -#"); // treat "generator-version" same as "generator version"
        struct GenRunInfo::ToolInfo generator = {parsed1.size()>0?parsed1.at(0):"Unknown", parsed1.size()>1?parsed1.at(1):"0.0.0", std::string("Used generator")};
        run_info()->tools().push_back(generator);
        std::string toparse2 = m_header.at(2);
        size_t reaction_b = toparse2.find_first_of("(");
        size_t reaction_e = toparse2.find_last_of(")")+1;
        if (reaction_e > reaction_b + 1) {
            std::string reaction = toparse2.substr(reaction_b,reaction_e-reaction_b);
            toparse2.erase(reaction_b,reaction_e-reaction_b);
            std::vector<std::string> parsed2 = tokenize_string(toparse2, " #");
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
