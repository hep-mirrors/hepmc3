/**
 *  @file protobufUtils.cc
 *  @brief Implementationof utility functions for protobufIO
 *
 */
#include "HepMC3/protobufUtils.h"

#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/Data/GenVertexData.h"

#include "HepMC3/Version.h"

namespace HepMC3 {

namespace Serialize {

template <typename T> std::string PBObjToString(T const &o) {
    std::string ostr;
    o.SerializeToString(&ostr);
    return ostr;
}

std::string GenRunInfo(HepMC3::GenRunInfo const &run_info) {
    GenRunInfoData data;
    run_info.write_data(data);

    HepMC3_pb::GenRunInfoData gri_pb;

    for (auto const &s : data.weight_names) {
        gri_pb.add_weight_names(s);
    }

    for (auto const &s : data.tool_name) {
        gri_pb.add_tool_name(s);
    }
    for (auto const &s : data.tool_version) {
        gri_pb.add_tool_version(s);
    }
    for (auto const &s : data.tool_description) {
        gri_pb.add_tool_description(s);
    }

    for (auto const &s : data.attribute_name) {
        gri_pb.add_attribute_name(s);
    }
    for (auto const &s : data.attribute_string) {
        gri_pb.add_attribute_string(s);
    }

    return PBObjToString(gri_pb);
}

std::string GenEvent(HepMC3::GenEvent const &evt) {
    GenEventData data;
    evt.write_data(data);

    HepMC3_pb::GenEventData ged_pb;
    ged_pb.set_event_number(data.event_number);

    switch (data.momentum_unit) {
    case HepMC3::Units::MEV: {
        ged_pb.set_momentum_unit(HepMC3_pb::GenEventData::MEV);
        break;
    }
    case HepMC3::Units::GEV: {
        ged_pb.set_momentum_unit(HepMC3_pb::GenEventData::GEV);
        break;
    }
    default: {
        HEPMC3_ERROR_LEVEL(300,"Unknown momentum unit: " << data.momentum_unit)
        return "ERROR";
    }
    }

    switch (data.length_unit) {
    case HepMC3::Units::MM: {
        ged_pb.set_length_unit(HepMC3_pb::GenEventData::MM);
        break;
    }
    case HepMC3::Units::CM: {
        ged_pb.set_length_unit(HepMC3_pb::GenEventData::CM);
        break;
    }
    default: {
        HEPMC3_ERROR_LEVEL(300,"Unknown length unit: " << data.length_unit)
        return "ERROR";
    }
    }

    for (auto const &pdata : data.particles) {
        auto* particle_pb = ged_pb.add_particles();
        particle_pb->set_pid(pdata.pid);
        particle_pb->set_status(pdata.status);
        particle_pb->set_is_mass_set(pdata.is_mass_set);
        particle_pb->set_mass(pdata.mass);

        particle_pb->mutable_momentum()->set_m_v1(pdata.momentum.x());
        particle_pb->mutable_momentum()->set_m_v2(pdata.momentum.y());
        particle_pb->mutable_momentum()->set_m_v3(pdata.momentum.z());
        particle_pb->mutable_momentum()->set_m_v4(pdata.momentum.t());
    }

    for (auto const &vdata : data.vertices) {
        auto* vertex_pb = ged_pb.add_vertices();
        vertex_pb->set_status(vdata.status);

        vertex_pb->mutable_position()->set_m_v1(vdata.position.x());
        vertex_pb->mutable_position()->set_m_v2(vdata.position.y());
        vertex_pb->mutable_position()->set_m_v3(vdata.position.z());
        vertex_pb->mutable_position()->set_m_v4(vdata.position.t());
    }

    for (auto const &s : data.weights) {
        ged_pb.add_weights(s);
    }

    for (auto const &s : data.links1) {
        ged_pb.add_links1(s);
    }
    for (auto const &s : data.links2) {
        ged_pb.add_links2(s);
    }

    ged_pb.mutable_event_pos()->set_m_v1(data.event_pos.x());
    ged_pb.mutable_event_pos()->set_m_v2(data.event_pos.y());
    ged_pb.mutable_event_pos()->set_m_v3(data.event_pos.z());
    ged_pb.mutable_event_pos()->set_m_v4(data.event_pos.t());

    for (auto const &s : data.attribute_id) {
        ged_pb.add_attribute_id(s);
    }
    for (auto const &s : data.attribute_name) {
        ged_pb.add_attribute_name(s);
    }
    for (auto const &s : data.attribute_string) {
        ged_pb.add_attribute_string(s);
    }
    return PBObjToString(ged_pb);
}

} // namespace Serialize

namespace Deserialize {

void FillGenRunInfo(HepMC3_pb::GenRunInfoData const &gri_pb,
                    std::shared_ptr<HepMC3::GenRunInfo> run_info) {

    HepMC3::GenRunInfoData gridata;

    int vector_size = 0;

    vector_size = gri_pb.weight_names_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.weight_names.push_back(gri_pb.weight_names(it));
    }

    vector_size = gri_pb.tool_name_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.tool_name.push_back(gri_pb.tool_name(it));
    }

    vector_size = gri_pb.tool_version_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.tool_version.push_back(gri_pb.tool_version(it));
    }

    vector_size = gri_pb.tool_description_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.tool_description.push_back(gri_pb.tool_description(it));
    }

    vector_size = gri_pb.attribute_name_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.attribute_name.push_back(gri_pb.attribute_name(it));
    }

    vector_size = gri_pb.attribute_string_size();
    for (int it = 0; it < vector_size; ++it) {
        gridata.attribute_string.push_back(gri_pb.attribute_string(it));
    }

    run_info->read_data(gridata);
}

bool GenRunInfo(std::string const &msg,
                std::shared_ptr<HepMC3::GenRunInfo> run_info) {
    if (!run_info) { // elide work because we have nowhere to put it
        return true;
    }

    HepMC3_pb::GenRunInfoData gri_pb;
    if (!gri_pb.ParseFromString(msg)) {
        return false;
    }

    FillGenRunInfo(gri_pb, run_info);

    return true;
}

void FillGenEvent(HepMC3_pb::GenEventData const &ged_pb,
                  HepMC3::GenEvent &evt) {

    HepMC3::GenEventData evtdata;
    evtdata.event_number = ged_pb.event_number();

    switch (ged_pb.momentum_unit()) {
    case HepMC3_pb::GenEventData::MEV: {
        evtdata.momentum_unit = HepMC3::Units::MEV;
        break;
    }
    case HepMC3_pb::GenEventData::GEV: {
        evtdata.momentum_unit = HepMC3::Units::GEV;
        break;
    }
    default: {
        HEPMC3_ERROR_LEVEL(300,"Unknown momentum unit: " << ged_pb.momentum_unit())
        return;
    }
    }

    switch (ged_pb.length_unit()) {
    case HepMC3_pb::GenEventData::MM: {
        evtdata.length_unit = HepMC3::Units::MM;
        break;
    }
    case HepMC3_pb::GenEventData::CM: {
        evtdata.length_unit = HepMC3::Units::CM;
        break;
    }
    default: {
        HEPMC3_ERROR_LEVEL(300,"Unknown length unit: " << ged_pb.length_unit())
        return;
    }
    }

    int vector_size = 0;

    evtdata.particles.clear();
    vector_size = ged_pb.particles_size();
    for (int it = 0; it < vector_size; ++it) {
        const auto& particle_pb = ged_pb.particles(it);

        HepMC3::GenParticleData pdata;

        pdata.pid = particle_pb.pid();
        pdata.status = particle_pb.status();
        pdata.is_mass_set = particle_pb.is_mass_set();
        pdata.mass = particle_pb.mass();

        pdata.momentum = HepMC3::FourVector{
            particle_pb.momentum().m_v1(), particle_pb.momentum().m_v2(),
            particle_pb.momentum().m_v3(), particle_pb.momentum().m_v4()};

        evtdata.particles.push_back(pdata);
    }

    evtdata.vertices.clear();
    vector_size = ged_pb.vertices_size();
    for (int it = 0; it < vector_size; ++it) {
        const auto& vertex_pb = ged_pb.vertices(it);

        HepMC3::GenVertexData vdata;

        vdata.status = vertex_pb.status();

        vdata.position = HepMC3::FourVector{
            vertex_pb.position().m_v1(), vertex_pb.position().m_v2(),
            vertex_pb.position().m_v3(), vertex_pb.position().m_v4()};

        evtdata.vertices.push_back(vdata);
    }

    evtdata.weights.clear();
    vector_size = ged_pb.weights_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.weights.push_back(ged_pb.weights(it));
    }

    evtdata.links1.clear();
    vector_size = ged_pb.links1_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.links1.push_back(ged_pb.links1(it));
    }

    evtdata.links2.clear();
    vector_size = ged_pb.links2_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.links2.push_back(ged_pb.links2(it));
    }

    evtdata.event_pos =
        HepMC3::FourVector{ged_pb.event_pos().m_v1(), ged_pb.event_pos().m_v2(),
                           ged_pb.event_pos().m_v3(), ged_pb.event_pos().m_v4()};

    evtdata.attribute_id.clear();
    vector_size = ged_pb.attribute_id_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.attribute_id.push_back(ged_pb.attribute_id(it));
    }

    evtdata.attribute_name.clear();
    vector_size = ged_pb.attribute_name_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.attribute_name.push_back(ged_pb.attribute_name(it));
    }

    evtdata.attribute_string.clear();
    vector_size = ged_pb.attribute_string_size();
    for (int it = 0; it < vector_size; ++it) {
        evtdata.attribute_string.push_back(ged_pb.attribute_string(it));
    }

    evt.read_data(evtdata);
}

bool GenEvent(std::string const &msg, HepMC3::GenEvent &evt) {

    HepMC3_pb::GenEventData ged_pb;
    if (!ged_pb.ParseFromString(msg)) {
        return false;
    }

    FillGenEvent(ged_pb, evt);

    return true;
}

} // namespace Deserialize

void GenEvent::read_data(HepMC3_pb::GenEventData const &data) {
    this->clear();
    this->set_event_number(data.event_number());

    // Note: set_units checks the current unit of event, i.e. applicable only for
    // fully constructed event.
    switch (data.momentum_unit()) {
    case HepMC3_pb::GenEventData::MEV: {
        m_momentum_unit = HepMC3::Units::MEV;
        break;
    }
    case HepMC3_pb::GenEventData::GEV: {
        m_momentum_unit = HepMC3::Units::GEV;
        break;
    }
    }

    switch (data.length_unit()) {
    case HepMC3_pb::GenEventData::MM: {
        m_length_unit = HepMC3::Units::MM;
        break;
    }
    case HepMC3_pb::GenEventData::CM: {
        m_length_unit = HepMC3::Units::CM;
        break;
    }
    }

    this->shift_position_to(
        HepMC3::FourVector{data.event_pos().m_v1(), data.event_pos().m_v2(),
                           data.event_pos().m_v3(), data.event_pos().m_v4()});

    // Fill weights
    m_weights.resize(data.weights_size());
    std::copy(data.weights().begin(), data.weights().end(), m_weights.begin());

    m_particles.reserve(data.particles_size());
    m_vertices.reserve(data.vertices_size());

    // Fill particle information
    for (auto const &pd : data.particles()) {
        m_particles.emplace_back(std::make_shared<GenParticle>(pd));
        m_particles.back()->m_event = this;
        m_particles.back()->m_id = m_particles.size();
    }

    // Fill vertex information
    for (auto const &vd : data.vertices()) {
        m_vertices.emplace_back(std::make_shared<GenVertex>(vd));
        m_vertices.back()->m_event = this;
        m_vertices.back()->m_id = -(int)m_vertices.size();
    }

    // Restore links
    for (unsigned int i = 0; i < (unsigned int)data.links1_size(); ++i) {
        const int id1 = data.links1(i);
        const int id2 = data.links2(i);
        /* @note:
        The  meaningfull combinations for (id1,id2) are:
        (+-)  --  particle has end vertex
        (-+)  --  particle  has production vertex
        */
        if ((id1 < 0 && id2 < 0) || (id1 > 0 && id2 > 0)) {
            HEPMC3_WARNING_LEVEL(600,"GenEvent::read_data: wrong link: " << id1 << " " << id2)
            continue;
        }

        if (id1 > 0) {
            m_vertices[(-id2) - 1]->add_particle_in(m_particles[id1 - 1]);
            continue;
        }
        if (id1 < 0) {
            m_vertices[(-id1) - 1]->add_particle_out(m_particles[id2 - 1]);
            continue;
        }
    }
    for (auto &p : m_particles) {
        if (!p->production_vertex()) m_rootvertex->add_particle_out(p);
    }
    // Read attributes
    std::lock_guard<std::recursive_mutex> lock(m_lock_attributes);
    for (unsigned int i = 0; i < (unsigned int)data.attribute_id_size(); ++i) {
        /// Disallow empty strings
        const std::string& name = data.attribute_name(i);
        if (name.length() == 0) {continue;}
        const int id = data.attribute_id(i);
        if (m_attributes.count(name) == 0) { m_attributes[name] = std::map<int, std::shared_ptr<Attribute>>(); }
        auto att = std::make_shared<StringAttribute>(data.attribute_string(i));
        att->m_event = this;
        if (id > 0 && id <= int(m_particles.size())) {
            att->m_particle = m_particles[id - 1];
        }
        if (id < 0 && -id <= int(m_vertices.size())) {
            att->m_vertex = m_vertices[-id - 1];
        }
        m_attributes[name][id] = att;
    }
}

GenVertex::GenVertex(HepMC3_pb::GenEventData_GenVertexData const &data)
    : m_event(nullptr), m_id(0) {
    m_data.status = data.status();
    m_data.position =
        HepMC3::FourVector{data.position().m_v1(), data.position().m_v2(),
                           data.position().m_v3(), data.position().m_v4()};
}

GenParticle::GenParticle(HepMC3_pb::GenEventData_GenParticleData const &data)
    : m_event(nullptr), m_id(0) {
    m_data.pid = data.pid();
    m_data.momentum =
        HepMC3::FourVector{data.momentum().m_v1(), data.momentum().m_v2(),
                           data.momentum().m_v3(), data.momentum().m_v4()};
    m_data.status = data.status();
    m_data.is_mass_set = data.is_mass_set();
    m_data.mass = data.mass();
}

} // namespace HepMC3