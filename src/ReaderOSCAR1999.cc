// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2024 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderOSCAR1999.cc
/// @brief Implementation of \b class ReaderOSCAR1999
///
#include <cstring>
#include <sstream>
#include <cassert>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Print.h"
#include "HepMC3/ReaderOSCAR1999.h"


namespace HepMC3 {

std::vector<std::string> tokenize_string(const std::string& str, const std::string& delimiters );

ReaderOSCAR1999::ReaderOSCAR1999(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false), m_OSCARType(0)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderOSCAR1999: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1999::ReaderOSCAR1999(std::istream & stream)
    : m_stream(&stream), m_isstream(true), m_OSCARType(0)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderOSCAR1999: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderOSCAR1999::ReaderOSCAR1999(std::shared_ptr<std::istream> s_stream)
    : m_shared_stream(s_stream), m_stream(s_stream.get()), m_isstream(true)
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
    /// So far this is not implemented
    return false;
}

bool ReaderOSCAR1999::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;
    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];
    evt.clear();
    m_vertices.clear();
    m_prod.clear();
    evt.set_run_info(run_info());
    bool file_header_parsed = false;
    int n_particles_expected = 0;
    int n_particles_parsed = 0;
    GenVertexPtr top = std::make_shared<GenVertex>();
    /// OSCAR has fixed units fm/GEV, right?
    const double from_fm = (evt.length_unit() == Units::MM) ? 1e-12 : 1e-13;
    const double from_gev = (evt.momentum_unit() == Units::GEV) ? 1 : 1e+3;
    while (!failed()) {
        m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
        if ( strlen(buf) == 0 ) continue;
        if ( strncmp(buf, "\r", 1) == 0 ) continue;
        /// Check for ReaderOSCAR1999 header/footer
        if ( strncmp(buf, "#", 1) == 0 ) {
            auto parsed = tokenize_string(std::string(buf), " #\r\n");
            if (parsed.size() == 1 ) {
                if ( parsed.at(0) == "OSC1999A" ) { m_header.clear(); continue;}
                /// Check for the type of content
                if ( parsed.at(0) == "final_id_p_x" ) m_OSCARType = 1;
                if ( parsed.at(0) == "full_event_history" ) m_OSCARType = 2;
            }
            m_header.push_back(std::string(buf));
            if (!file_header_parsed) file_header_parsed = parse_header();
            continue;
        }
        auto interaction_header_tokens = tokenize_string(std::string(buf), " \r\n");
        if (interaction_header_tokens.size() < 2) { HEPMC3_ERROR("Unexpected string"<< buf); continue; }
        if (interaction_header_tokens.at(0) == "0" && interaction_header_tokens.at(1) == "0") {
            /// This should be the end of event
            /// Parce the content of string to find the event number and HI information
            if (interaction_header_tokens.size() > 2) evt.set_event_number(atoi(interaction_header_tokens.at(2).c_str()));
            if (interaction_header_tokens.size() > 3) {
                GenHeavyIonPtr  hi = std::make_shared<GenHeavyIon>();
                hi->set(-1, -1, -1, -1, -1, -1,
                        -1, -1, -1,
                        -1.0, -1.0, -1.0, -1.0, -1.0, -1.0);
                hi->impact_parameter = std::strtof(interaction_header_tokens.at(3).c_str(),nullptr);
                evt.set_heavy_ion(hi);
            }
            /// Set the name for the only weight
            std::vector<std::string> weightnames;
            weightnames.push_back("Default");
            run_info()->set_weight_names(weightnames);
            /// Set dummy cross-section info
            GenCrossSectionPtr cs = std::make_shared<GenCrossSection>();
            cs->set_cross_section(1.0,1.0);
            evt.set_cross_section(cs);
            /// Set weights
            evt.weights() = std::vector<double>(1,1);
            evt.set_run_info(run_info());

            if ( m_OSCARType == 1 ) {
                /// Add the interaction vertex to the event.
                evt.add_vertex(top);
                if (n_particles_parsed != n_particles_expected) HEPMC3_ERROR("ReaderOSCAR1999: wrong number of particles parsed"<<n_particles_parsed << "vs expected "<< n_particles_expected);
                /// Done
                return true;
            }

            if (m_OSCARType == 2) {
                /// Manipulate with history and remove duplicated particles, i.e. those that recorded as "in" and "out"
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
                /// Done
                return true;
            }
            break;
        }

        if (m_OSCARType == 1) {
            /// The expected content is the event header
            int nin = atoi(interaction_header_tokens.at(1).c_str());
            int nout = atoi(interaction_header_tokens.at(0).c_str());
            n_particles_expected = nout;
            if (interaction_header_tokens.size() > 2) evt.set_event_number(atoi(interaction_header_tokens.at(2).c_str()));
            if (nin == 0 && nout != 0) {
                /// Add dummy particles as beams
                GenParticlePtr b1 =std::make_shared<GenParticle>();
                evt.add_beam_particle(b1);
                top->add_particle_in(b1);
                GenParticlePtr b2 =std::make_shared<GenParticle>();
                evt.add_beam_particle(b2);
                top->add_particle_in(b2);
                evt.add_vertex(top);
            }
        }

        if (m_OSCARType == 1) {
            /// The expected content is particle in a form
            /// id pdg 0 px py pz p0 mass x y z t
            /// e.g.
            /// 2161 2112 0 -0.176933 -0.0777044 -0.0590065 0.959515 0.938 -34.9518 -17.0731 -0.903103 200
            int i[3];
            double d[9];
            sscanf(buf,"%i %i %i %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", i, i+1, i+2, d, d+1, d+2, d+3, d+4, d+5, d+6, d+7, d+8);
            /// Create two particles. One for the final state and the other to connect the "interaction" vertex with the "final" vertex
            GenParticlePtr in = std::make_shared<GenParticle>(FourVector(from_gev*d[0], from_gev*d[1], from_gev*d[2], from_gev*d[3]),i[1],3);
            GenParticlePtr out = std::make_shared<GenParticle>(FourVector(from_gev*d[0], from_gev*d[1], from_gev*d[2], from_gev*d[3]),i[1],1);
            in->set_generated_mass(d[4]);
            out->set_generated_mass(d[4]);
            /// Create "final" vertex
            GenVertexPtr v = std::make_shared<GenVertex>(FourVector(from_fm*d[5], from_fm*d[6],  from_fm*d[7], from_fm*d[8]));
            v->add_particle_in(in);
            v->add_particle_out(out);
            top->add_particle_out(in);
            evt.add_particle(in);
            evt.add_particle(out);
            evt.add_vertex(v);
            n_particles_parsed++;
            continue;
        }
        if (m_OSCARType == 2) {
/// The expected content is interation
            int nin = atoi(interaction_header_tokens.at(0).c_str());
            int nout = atoi(interaction_header_tokens.at(1).c_str());
            GenVertexPtr v = std::make_shared<GenVertex>();
///OSCAR1999A is implemented inconsistently across the generators.
///UrqMD has three versions, the input the interaction and the output
///->      0       24        1     0.971     0.000<-
/// UrQd amnual page 33
///->          2        2        1        1   1.577  0.2611E+01  0.4212E+02  0.1035E+02  0.1221E+00<-
/// UrQd amnual page 27
///->      32        0<-
/// Not documented
///SMASH has only interaction format
///->2 2    0.0000000   45.2577226   21.0598335     1<-
            for (int j = 0; j < nin + nout; j++ ) {
                m_isstream ? m_stream->getline(buf, max_buffer_size) : m_file.getline(buf, max_buffer_size);
                if ( strlen(buf) == 0 ) {
                    HEPMC3_ERROR("ReaderOSCAR1999: event parsing failed. Returning empty event")
                    return false;
                }
                int i[3];
                double d[9];
                sscanf(buf,"%i %i %i %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", i, i+1, i+2, d, d+1, d+2, d+3, d+4, d+5, d+6,  d+7, d+8);
                GenParticlePtr p = std::make_shared<GenParticle>(FourVector(from_gev*d[0],from_gev*d[1],from_gev*d[2],from_gev*d[3]),i[0],3);
                p->set_generated_mass(from_gev*d[4]);
                FourVector fv(from_fm*d[5],from_fm*d[6],from_fm*d[7],from_fm*d[8]);
                if (!j && nin ) v->set_position(fv);
                if (j<nin) { v->add_particle_in(p); }
                else { v->add_particle_out(p); }
                m_prod[i[0]].push_back(std::pair<GenParticlePtr,FourVector>(p,fv));
            }
            /// Initial vertex, containing the beam particles
            if (nin == 0 && nout != 0 ) {
                auto particles_out = v->particles_out();
                for (auto p: particles_out)
                {
                    v->remove_particle_out(p);
                    p->set_status(4);
                    evt.add_beam_particle(p);
                }
                /// The meaning of the information here is also different
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

            /// A fake vertex for the stable particles
            if (nin != 0 && nout == 0 ) {
                auto particles_final = v->particles_in();
                for (auto p: particles_final)
                {
                    v->remove_particle_in(p);
                    p->set_status(1);
                    evt.add_particle(p);
                }
            }

            /// A normal interaction vertex
            if (nin !=0 && nout !=0 )
            {
                evt.add_vertex(v);
                /// The information seems to be generator-dependent
                if (interaction_header_tokens.size() == 9 )
                {
                    v->add_attribute("process_type", std::make_shared<IntAttribute>(atoi(interaction_header_tokens.at(2).c_str())));
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
    }
    return true;
}

bool ReaderOSCAR1999::parse_header() {
    if (m_header.size() < 3) return false;
    for (auto &s: m_header) s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
    run_info()->add_attribute("content", std::make_shared<StringAttribute>(m_header.at(0).substr(2)));
    std::string toparse1 = m_header.at(1);
    auto parsed1 = tokenize_string(toparse1, " -#\r\n"); // treat "generator-version" same as "generator version"
    struct GenRunInfo::ToolInfo generator = {parsed1.size()>0?parsed1.at(0):"Unknown", parsed1.size()>1?parsed1.at(1):"0.0.0", std::string("Used generator")};
    run_info()->tools().push_back(generator);
    std::string toparse2 = m_header.at(2);
    size_t reaction_b = toparse2.find_first_of("(");
    size_t reaction_e = toparse2.find_last_of(")") + 1;
    if (reaction_e > reaction_b + 1) {
        std::string reaction = toparse2.substr(reaction_b, reaction_e - reaction_b);
        run_info()->add_attribute("reaction", std::make_shared<StringAttribute>(reaction));
        toparse2.erase(reaction_b, reaction_e - reaction_b);
        auto parsed2 = tokenize_string(toparse2, " #\r\n");
        if (parsed2.size() > 0) run_info()->add_attribute("reference_frame", std::make_shared<StringAttribute>(parsed2.at(0)));
        if (parsed2.size() > 1) run_info()->add_attribute("beam_energy", std::make_shared<DoubleAttribute>(std::strtof(parsed2.at(1).c_str(),NULL)));
        if (parsed2.size() > 2) run_info()->add_attribute("test_particles_per_nuclon", std::make_shared<IntAttribute>(std::atoi(parsed2.at(2).c_str())));
    }
    return true;
}

bool ReaderOSCAR1999::failed() {
    return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

void ReaderOSCAR1999::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}

} // namespace HepMC3
