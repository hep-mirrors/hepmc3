// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR2013.cc
/// @brief Implementation of \b class ReaderOSCAR2013
///
#include <cstring>
#include <sstream>
#include <cassert>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"
#include "HepMC3/ReaderOSCAR2013.h"


namespace HepMC3 {


ReaderOSCAR2013::ReaderOSCAR2013(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false), m_OSCARType(0)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR2013: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR2013::ReaderOSCAR2013(std::istream & stream)
    : m_stream(&stream), m_isstream(true), m_OSCARType(0)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR2013: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR2013::~ReaderOSCAR2013() {
    if (!m_isstream) close();
}

bool ReaderOSCAR2013::skip(const int n)
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
bool ReaderOSCAR2013::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    const char                 *cursor   = buf;
    evt.clear();
    m_vertices.clear();
    m_prod.clear();
    evt.set_units(Units::GEV,Units::MM);//Fixed units?
    evt.set_run_info(run_info());
    bool file_header_parsed = false;
    int n_particles_expected = 0;
    int n_particles_parsed = 0;
    GenVertexPtr top = std::make_shared<GenVertex>();

    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        cursor = buf;
        if ( strlen(buf) == 0 ) continue;
        if ( strncmp(buf, "#", 1) == 0 && strncmp(buf, "# interaction", 13) !=0  ) {
            std::vector<std::string> parsed = tokenize_string(std::string(buf), " #");
            if (parsed.size() == 14 ) {
                if ( parsed.at(0) == "!OSCAR2013" && parsed.at(1) == "particle_lists" ) m_OSCARType = 1;
                if ( parsed.at(0) == "!OSCAR2013" && parsed.at(1) == "full_event_history" ) { m_OSCARType = 2;  HEPMC3_ERROR("Not fully supportded so far"); }
                continue;
            }
            if (parsed.size() == 4 &&  m_OSCARType == 1 ) {
                //start of event!
                // We add two particles, despite it is actually not needed.
                GenParticlePtr b1 =std::make_shared<GenParticle>();
                evt.add_beam_particle(b1);
                top->add_particle_in(b1);
                GenParticlePtr b2 =std::make_shared<GenParticle>();
                evt.add_beam_particle(b2);
                top->add_particle_in(b2);
                evt.add_vertex(top);

                evt.set_event_number(atoi(parsed.at(1).c_str()));
                n_particles_expected = atoi(parsed.at(3).c_str());
                continue;
            }
            if (parsed.size() == 8 && ( m_OSCARType == 1 || m_OSCARType == 2 )) {
                //end of event!
                GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
                hi->set(-1, -1, -1, -1, -1, -1,
                        -1, -1, -1,
                        -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
                GenCrossSectionPtr cs = std::make_shared<GenCrossSection>();
                hi->impact_parameter = std::strtof(parsed.at(5).c_str(),nullptr);
                cs->set_cross_section(1.0,1.0);
                evt.set_cross_section(cs);
                evt.set_heavy_ion(hi);
                evt.set_run_info(run_info());
                evt.weights() = std::vector<double>(1,1);

                if ( m_OSCARType == 1 ) {
                    evt.add_vertex(top);
                    if (n_particles_parsed != n_particles_expected) HEPMC3_ERROR("ReaderOSCAR2013: wrong number of particles parsed"<<n_particles_parsed << "vs expected "<< n_particles_expected);
                }
                if ( m_OSCARType == 2 ) {
                    for (auto& pp: m_prod)
                    {
                        auto& history = pp.second;
                        ///printf("/////////////// %i\n",pp.first);
                        //for (auto a: history) HepMC3::Print::line(a.first);
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
                    for (auto p: evt.particles()) if (!p->end_vertex() ) p->set_status(1);
                }
                break;
            }
            if (parsed.size() == 0) continue;
            if (m_header.size() < 2) {
                m_header.push_back(std::string(buf));
                if (m_header.size() == 2 && !file_header_parsed) {
                    run_info()->add_attribute("units", std::make_shared<StringAttribute>(m_header.at(0)));
                    std::vector<std::string> parsed1 = tokenize_string(m_header.at(1), "- #");
                    struct GenRunInfo::ToolInfo generator = {parsed1.size()>0?parsed1.at(0):"Unknown", parsed1.size()>1?parsed1.at(1):"0.0.0", std::string("Used generator")};
                    run_info()->tools().push_back(generator);
                    std::vector<std::string> weightnames;
                    weightnames.push_back("Default");
                    run_info()->set_weight_names(weightnames);
                    file_header_parsed = true;
                }
                continue;
            }
            HEPMC3_ERROR("Unknown header line" << buf);
        }

        if (m_OSCARType == 1) {
            int i[4];
            double d[8];
            sscanf(buf,"%i %lf %lf %lf %lf %lf %lf %lf %lf %i %i %i\n", i, d, d+1, d+2, d+3, d+4, d+5, d+6,  d+7, i+1, i+2, i+3);
            double e = std::sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2] +d[3]*d[3]);
            GenParticlePtr in = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], e),i[1],3);
            GenParticlePtr out = std::make_shared<GenParticle>(FourVector(d[0], d[1], d[2], e),i[1],1);
            in->set_generated_mass(d[3]);
            out->set_generated_mass(d[3]);
            GenVertexPtr v = std::make_shared<GenVertex>(FourVector(d[4], d[5],  d[6],  d[7]));
            v->add_particle_in(in);
            v->add_particle_out(out);
            top->add_particle_out(in);
            evt.add_particle(in);
            evt.add_particle(out);
            evt.add_vertex(v);
            n_particles_parsed++;
        }
        if (m_OSCARType == 2) {
            if ( strncmp(buf, "#", 1) == 0 )
            {
                //# interaction in 2 out 2 rho    0.0000000 weight     46.76038 partial   17.8799439 type     3
                std::vector<std::string> interaction_header_tokens = tokenize_string(std::string(buf), " #");
                int nin = atoi (  interaction_header_tokens.at(2).c_str());
                int nout = atoi (  interaction_header_tokens.at(4).c_str());
                GenVertexPtr v = std::make_shared<GenVertex>();//FourVector(d[1], d[2],  d[3],  d[0]));

                for (int j = 0; j < nin + nout; j++ ) {
                    m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
                    if ( strlen(buf) == 0 ) {
                        HEPMC3_ERROR("ReaderOSCAR2013: event parsing failed. Returning empty event")
                        HEPMC3_DEBUG(1, "Parsing failed at line:" << std::endl << buf)
                    }
                    int i[3];
                    double d[9];
                    // #!OSCAR2013 particle_lists t x y z mass p0 px py pz pdg ID charge
                    sscanf(buf,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %i %i %i\n", d, d+1, d+2, d+3, d+4, d+5, d+6,  d+7, d+8,  i, i+1, i+2);
                    GenParticlePtr p = std::make_shared<GenParticle>(FourVector(d[6], d[7], d[8], d[5]),i[0],3);
                    p->set_generated_mass(d[4]);
                    FourVector fv(d[1], d[2],  d[3],  d[0]);
                    m_prod[i[1]].push_back(std::pair<GenParticlePtr,FourVector>(p,fv));

                    if (!j && nin ) v->set_position(fv);
                    if (j<nin) { v->add_particle_in(p); }
                    else { v->add_particle_out(p); }
                }
                m_vertices.push_back(v);
                if (nin == 0 && nout != 0 ) {
                    auto particles_out = v->particles_out();
                    for (auto p: particles_out)
                    {
                        v->remove_particle_out(p);
                        p->set_status(4);
                        evt.add_beam_particle(p);
                    }

                }
                if (nin !=0 && nout !=0 )
                {
                    evt.add_vertex(v);
                    if (interaction_header_tokens.size() > 12) {
                        v->add_attribute("baryon_density", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(6).c_str(),nullptr)));
                        v->add_attribute("total_weight", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(8).c_str(),nullptr)));
                        v->add_attribute("partial_weight", std::make_shared<DoubleAttribute>(std::strtof(interaction_header_tokens.at(10).c_str(),nullptr)));
                        v->add_attribute("process_type", std::make_shared<IntAttribute>(atoi(interaction_header_tokens.at(12).c_str())));
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
                continue;
            }
        }
    }
    return true;
}

bool ReaderOSCAR2013::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR2013::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
