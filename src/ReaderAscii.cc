// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderAscii.cc
/// @brief Implementation of \b class ReaderAscii
///
#include <array>
#include <cstring>
#include <sstream>

#include "HepMC3/ReaderAscii.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"

namespace HepMC3 {


ReaderAscii::ReaderAscii(const std::string &filename)
    : m_file(filename), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderAscii: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderAscii::ReaderAscii(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderAscii: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}


ReaderAscii::ReaderAscii(std::shared_ptr<std::istream> s_stream)
    : m_shared_stream(s_stream), m_stream(s_stream.get()), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderAscii: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderAscii::~ReaderAscii() { if (!m_isstream) close(); }

bool ReaderAscii::skip(const int n)
{
    std::array<char, 262144> buf{};
    bool               event_context    = false;
    bool               run_info_context    = false;
    int nn = n;
    while (!failed()) {
        char  peek(0);
        if ( (!m_file.is_open()) && (!m_isstream) ) return false;
        m_isstream ? peek = m_stream->peek() : peek = m_file.peek();
        if ( peek == 'E' ) { event_context = true; nn--; }
        //We have to read each run info.
        if ( !event_context && ( peek == 'W' || peek == 'A' || peek == 'T' ) ) {
            m_isstream ? m_stream->getline(buf.data(), buf.size()) : m_file.getline(buf.data(), buf.size());
            if (!run_info_context) {
                set_run_info(std::make_shared<GenRunInfo>());
                run_info_context = true;
            }
            if ( peek == 'W' ) {
                parse_weight_names(buf.data());
            }
            if ( peek == 'T' ) {
                parse_tool(buf.data());
            }
            if ( peek == 'A' ) {
                parse_run_attribute(buf.data());
            }
        }
        if ( event_context && ( peek == 'V' || peek == 'P' ) ) event_context=false;
        if (nn < 0) return true;
        m_isstream ? m_stream->getline(buf.data(), buf.size()) : m_file.getline(buf.data(), buf.size());
    }
    return true;
}


bool ReaderAscii::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;

    char               peek(0);
    std::array<char, 262144> buf{};
    bool               event_context    = false;
    bool               parsed_weights    = false;
    bool               parsed_particles_or_vertices    = false;
    bool               run_info_context    = false;
    bool               is_parsing_successful  = true;
    std::pair<int, int> vertices_and_particles(0, 0);

    evt.clear();
    evt.set_run_info(run_info());
    m_io_explicit.clear();
    m_io_implicit.clear();
    m_io_implicit_ids.clear();
    m_io_explicit_ids.clear();
    m_data.particles.clear();
    m_data.vertices.clear();
    m_data.links1.clear();
    m_data.links2.clear();
    m_data.attribute_id.clear();
    m_data.attribute_name.clear();
    m_data.attribute_string.clear();
    //
    // Parse event, vertex and particle information
    //
    while (!failed()) {
        m_isstream ? m_stream->getline(buf.data(), buf.size()) : m_file.getline(buf.data(), buf.size());

        if ( strlen(buf.data()) == 0 ) continue;

        // Check for ReaderAscii header/footer
        if ( strncmp(buf.data(), "HepMC", 5) == 0 ) {
            if ( strncmp(buf.data(), "HepMC::Version", 14) != 0 && strncmp(buf.data(), "HepMC::Asciiv3", 14) != 0 )
            {
                HEPMC3_WARNING("ReaderAscii: found unsupported expression in header. Will close the input.")
                std::cout << buf.data() << std::endl;
                m_isstream ? m_stream->clear(std::ios::eofbit) : m_file.clear(std::ios::eofbit);
            }
            if (event_context) {
                is_parsing_successful = true;
                break;
            }
            continue;
        }

        switch (buf[0]) {
        case 'E':
            vertices_and_particles = parse_event_information( buf.data());
            if (vertices_and_particles.second < 0) {
                is_parsing_successful = false;
            } else {
                is_parsing_successful = true;
                event_context   = true;
                parsed_weights = false;
                parsed_particles_or_vertices = false;
            }
            
            
            run_info_context   = false;
            break;
        case 'V':
            is_parsing_successful = parse_vertex_information( buf.data());
            parsed_particles_or_vertices =  true;
            break;
        case 'P':
            is_parsing_successful = parse_particle_information( buf.data());
            parsed_particles_or_vertices =  true;
            break;
        case 'W':
            if ( event_context ) {
                is_parsing_successful = parse_weight_values( buf.data());
                parsed_weights=true;
            } else {
                if ( !run_info_context ) {
                    set_run_info(std::make_shared<GenRunInfo>());
                    evt.set_run_info(run_info());
                }
                run_info_context = true;
                is_parsing_successful = parse_weight_names(buf.data());
            }
            break;
        case 'U':
            is_parsing_successful = parse_units( buf.data());
            break;
        case 'T':
            if ( event_context ) {
                //We ignore T in the event context
            } else {
                if ( !run_info_context ) {
                    set_run_info(std::make_shared<GenRunInfo>());
                    evt.set_run_info(run_info());
                }
                run_info_context = true;
                is_parsing_successful = parse_tool(buf.data());
            }
            break;
        case 'A':
            if ( event_context ) {
                is_parsing_successful = parse_attribute( buf.data());
            } else {
                if ( !run_info_context ) {
                    set_run_info(std::make_shared<GenRunInfo>());
                    evt.set_run_info(run_info());
                }
                run_info_context = true;
                is_parsing_successful = parse_run_attribute(buf.data());
            }
            break;
        default:
            HEPMC3_WARNING("ReaderAscii: skipping unrecognised prefix: " << buf[0])
            is_parsing_successful = true;
            break;
        }

        if ( !is_parsing_successful ) break;

        // Check for next event or run info
        m_isstream ? peek = m_stream->peek() : peek = m_file.peek();
        //End of event. The next entry is event.
        if ( event_context &&  peek == 'E' ) break;

        //End of event. The next entry is run info which starts from weight name.
        if ( event_context &&  peek == 'W' && parsed_weights  ) break;

        //End of event. The next entry is run info which starts from attribute.
        if ( event_context &&  peek == 'A' && parsed_particles_or_vertices  ) break;

        //End of event. The next entry is run info which starts from tool.
        if ( event_context &&  peek == 'T' ) break;

    }
    
    /// Insert the implicit vertices in the gaps of explicit vertices:
    /// Find the gaps looping over the explicit vertices
    int currid = -static_cast<int>(m_data.vertices.size());
    auto fir = m_io_implicit_ids.rbegin();
    for (const auto& iofirst: m_io_explicit_ids) {
        for (;currid < iofirst; ++currid, ++fir) {
            if (fir == m_io_implicit_ids.rend()) { HEPMC3_ERROR("ReaderAscii: not enough implicit vertices") }
            /// Found a gap in ids, insert an implicit vertex into a list of gaps.
            m_io_explicit[currid] = std::move(m_io_implicit[*fir]);
        }
        ++currid;
    }

    for (const auto& io: m_io_explicit) {
      for (const auto& i: io.second.first) { m_data.links1.push_back(i); m_data.links2.push_back(io.first); }
      for (const auto& o: io.second.second) { m_data.links1.push_back(io.first); m_data.links2.push_back(o); }
    }
    evt.read_data(m_data);

    // Check if all particles and vertices were parsed
    if ((int)evt.particles().size() > vertices_and_particles.second) {
        HEPMC3_ERROR("ReaderAscii: too many particles were parsed")
        printf("%zu  vs  %i expected\n", evt.particles().size(), vertices_and_particles.second);
        is_parsing_successful = false;
    }
    if ((int)evt.particles().size() < vertices_and_particles.second) {
        HEPMC3_ERROR("ReaderAscii: too few  particles were parsed")
        printf("%zu  vs  %i expected\n", evt.particles().size(), vertices_and_particles.second);
        is_parsing_successful = false;
    }

    if ((int)evt.vertices().size()  > vertices_and_particles.first) {
        HEPMC3_ERROR("ReaderAscii: too many vertices were parsed")
        printf("%zu  vs  %i expected\n", evt.vertices().size(), vertices_and_particles.first);
        is_parsing_successful =  false;
    }

    if ((int)evt.vertices().size()  < vertices_and_particles.first) {
        HEPMC3_ERROR("ReaderAscii: too few vertices were parsed")
        printf("%zu  vs  %i expected\n", evt.vertices().size(), vertices_and_particles.first);
        is_parsing_successful =  false;
    }
    // Check if there were HEPMC3_ERRORs during parsing
    if ( !is_parsing_successful ) {
        HEPMC3_ERROR("ReaderAscii: event parsing failed. Returning empty event")
        HEPMC3_DEBUG(1, "Parsing failed at line:" << std::endl << buf.data())

        evt.clear();
        m_isstream ? m_stream->clear(std::ios::badbit) : m_file.clear(std::ios::badbit);

        return false;
    }


    return true;
}


std::pair<int, int> ReaderAscii::parse_event_information(const char *buf) {
    static const std::pair<int, int>  err(-1, -1);
    std::pair<int, int>               ret(-1, -1);
    const char                 *cursor   = buf;
    FourVector&                  position = m_data.event_pos;

    // event number
    if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
    m_data.event_number = atoi(cursor);

    // num_vertices
    if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
    ret.first = atoi(cursor);

    // num_particles
    if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
    ret.second = atoi(cursor);
    m_data.vertices = std::vector<GenVertexData>(ret.first);
    m_data.particles = std::vector<GenParticleData>(ret.second);

    m_data.links1.reserve(ret.second*2);
    m_data.links2.reserve(ret.second*2);
    m_data.attribute_id.reserve(ret.second + ret.first);
    m_data.attribute_name.reserve(ret.second + ret.first);
    m_data.attribute_string.reserve(ret.second + ret.first);
    m_io_implicit_ids.reserve(ret.second);
    // check if there is position information
    if ( (cursor = strchr(cursor+1, '@')) ) {
        // x
        if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
        position.setX(atof(cursor));

        // y
        if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
        position.setY(atof(cursor));

        // z
        if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
        position.setZ(atof(cursor));

        // t
        if ( !(cursor = strchr(cursor+1, ' ')) ) return err;
        position.setT(atof(cursor));
    }

    HEPMC3_DEBUG(10, "ReaderAscii: E: " << m_data.event_number << " (" <<ret.first << "V, " << ret.second << "P)")

    return ret;
}


bool ReaderAscii::parse_weight_values(const char *buf) {
    std::istringstream iss(buf + 1);
    std::vector<double> wts;
    double w = 0.0;
    while (iss >> w) wts.emplace_back(w);
    if ( run_info() && !run_info()->weight_names().empty()
            && run_info()->weight_names().size() != wts.size() ) {
        throw std::logic_error("ReaderAscii::parse_weight_values: "
                               "The number of weights ("+std::to_string((long long int)(wts.size()))+") does not match "
                               "the  number weight names("+std::to_string((long long int)(run_info()->weight_names().size()))+") in the GenRunInfo object");
    }
    m_data.weights = wts;

    return true;
}


bool ReaderAscii::parse_units(const char *buf) {
    const char *cursor = buf;

    // momentum
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;
    m_data.momentum_unit = Units::momentum_unit(cursor);

    // length
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;
    m_data.length_unit = Units::length_unit(cursor);

    HEPMC3_DEBUG(10, "ReaderAscii: U: " << Units::name(m_data.momentum_unit) << " " << Units::name(m_data.length_unit))

    return true;
}


bool ReaderAscii::parse_vertex_information(const char *buf) {
    GenVertexPtr  data = std::make_shared<GenVertex>();
    const char   *cursor          = buf;
    const char   *cursor2         = nullptr;
    int           id              = 0;

    // id
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    id = atoi(cursor);

    // status
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    m_data.vertices[-id-1].status = atoi(cursor);
    FourVector&  position = m_data.vertices[-id-1].position;

    // skip to the list of particles
    if ( !(cursor = strchr(cursor+1, '[')) ) return false;

    while (true) {
        ++cursor;             // skip the '[' or ',' character
        cursor2     = cursor; // save cursor position
        int  particle_in = atoi(cursor);

        // add incoming particle to the vertex
        if (particle_in > 0) {
                //If the particle has not been red yet, we store its id to add the particle later.
                m_io_explicit[id].first.insert(particle_in);
        }

        // check for next particle or end of particle list
        if ( !(cursor = strchr(cursor+1, ',')) ) {
            if ( !(cursor = strchr(cursor2+1, ']')) ) return false;
            break;
        }
    }

    // check if there is position information
    if ( (cursor = strchr(cursor+1, '@')) ) {
        // x
        if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
        position.setX(atof(cursor));

        // y
        if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
        position.setY(atof(cursor));

        // z
        if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
        position.setZ(atof(cursor));

        // t
        if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
        position.setT(atof(cursor));
    }

    return true;
}


bool ReaderAscii::parse_particle_information(const char *buf) {
    const char     *cursor  = buf;
    int             mother_id = 0;

    // verify id
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;

    int id = atoi(cursor);
    if ( id < 1 || id > static_cast<int>(m_data.particles.size()) ) {
        HEPMC3_ERROR("ReaderAscii: particle ID is out of expected range.")
        return false;
    }

    FourVector&      momentum = m_data.particles[id-1].momentum;
    // mother id
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    mother_id = atoi(cursor);
    if ( mother_id < -static_cast<int>(m_data.vertices.size()) || mother_id > static_cast<int>(m_data.particles.size()) ) {
        HEPMC3_ERROR("ReaderAscii: ID of particle mother is out of expected range.")
        return false;
    }

    if ( mother_id > 0) {
        /// Parent object is a particle, i.e. the vertex is implicit.
        /// If the vertex is not known -- mark its first appearence.
        if (m_io_implicit.count(mother_id) == 0) m_io_implicit_ids.push_back(mother_id);
        m_io_implicit[mother_id].first.insert(mother_id);
        m_io_implicit[mother_id].second.insert(id);
    } else {
      m_io_explicit[mother_id].second.insert(id);
      m_io_explicit_ids.insert(mother_id);
    }
    // pdg id
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    m_data.particles[id-1].pid = atoi(cursor);

    // px
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    momentum.setPx(atof(cursor));

    // py
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    momentum.setPy(atof(cursor));

    // pz
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    momentum.setPz(atof(cursor));

    // pe
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    momentum.setE(atof(cursor));

    // m
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    m_data.particles[id-1].mass = atof(cursor);
    m_data.particles[id-1].is_mass_set = true;

    // status
    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    m_data.particles[id-1].status = atoi(cursor);

    return true;
}


bool ReaderAscii::parse_attribute(const char *buf) {
    const char     *cursor  = buf;
    const char     *cursor2 = buf;
    std::array<char, 512> name{};
    int             id = 0;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    id = atoi(cursor);

    if ( !(cursor  = strchr(cursor+1, ' ')) ) return false;
    ++cursor;

    if ( !(cursor2 = strchr(cursor, ' ')) ) return false;
    snprintf(name.data(), name.size(), "%.*s", (int)(cursor2-cursor), cursor);

    cursor = cursor2+1;

    m_data.attribute_id.push_back(id);
    m_data.attribute_name.emplace_back(name.data());
    m_data.attribute_string.push_back(unescape(cursor));

    return true;
}

bool ReaderAscii::parse_run_attribute(const char *buf) {
    const char     *cursor  = buf;
    const char     *cursor2 = buf;
    std::array<char, 512> name{};

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;

    if ( !(cursor2 = strchr(cursor, ' ')) ) return false;
    snprintf(name.data(), name.size(), "%.*s", (int)(cursor2-cursor), cursor);

    cursor = cursor2+1;

    std::shared_ptr<StringAttribute> att =
        std::make_shared<StringAttribute>(StringAttribute(unescape(cursor)));

    run_info()->add_attribute(std::string(name.data()), att);

    return true;
}


bool ReaderAscii::parse_weight_names(const char *buf) {
    const char     *cursor  = buf;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;

    std::istringstream iss(unescape(cursor));
    std::vector<std::string> names;
    std::string name;
    while (iss >> name) names.emplace_back(name);

    run_info()->set_weight_names(names);

    return true;
}

bool ReaderAscii::parse_tool(const char *buf) {
    const char     *cursor  = buf;

    if ( !(cursor = strchr(cursor+1, ' ')) ) return false;
    ++cursor;
    std::string line = unescape(cursor);
    GenRunInfo::ToolInfo tool;
    std::string::size_type pos = line.find('\n');
    tool.name = line.substr(0, pos);
    line = line.substr(pos + 1);
    pos = line.find('\n');
    tool.version = line.substr(0, pos);
    tool.description = line.substr(pos + 1);
    run_info()->tools().emplace_back(tool);

    return true;
}


std::string ReaderAscii::unescape(const std::string& s) {
    std::string ret;
    ret.reserve(s.length());
    for ( std::string::const_iterator it = s.begin(); it != s.end(); ++it ) {
        if ( *it == '\\' ) {
            ++it;
            if ( *it == '|' ) {
                ret += '\n';
            }
            else {
                ret += *it;
            }
        } else
        {ret += *it;}
    }

    return ret;
}

bool ReaderAscii::failed() { return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate(); }

void ReaderAscii::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}


} // namespace HepMC3
