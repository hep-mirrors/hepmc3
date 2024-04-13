// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2024 The HepMC collaboration (see AUTHORS for details)
//
#include "ReaderuprootTree.h"
using namespace pybind11::literals;
namespace HepMC3
{
template<class T>
std::vector<T>  make_vector_from_1d_numpy_array( const pybind11::array_t<T>& py_array ) {
    return std::vector<T>(py_array.data(), py_array.data() + py_array.size());
}

/// @brief obtain vector of objects using name and type
template <class T> std::vector<T>
ReaderuprootTree::get_vector(pybind11::object& tr, const std::string& array_name) {
    int i = m_events_count;
    auto x0 = tr[array_name.c_str()].attr("array")("library"_a="np").attr("item")(i);
    auto x = m_numpy_module.attr("array")(x0);
    std::vector<T> out =  make_vector_from_1d_numpy_array<T>(x);
    return out;
}

/// @brief obtain vector of objects using name and type, specified for std::string
template <>
std::vector<std::string>
ReaderuprootTree::get_vector<std::string>(pybind11::object& tr, const std::string& array_name) {
    int i = m_events_count;
    auto x0 = tr[array_name.c_str()].attr("array")("library"_a="np").attr("item")(i);
    auto x = m_numpy_module.attr("array")(x0,"dtype"_a="U500"); 
    std::vector<std::array<char,500>> out2 =  make_vector_from_1d_numpy_array<std::array<char,500>>(x);
    std::vector<std::string> out;
    out.reserve(out2.size());
    for (const auto& a: out2) out.emplace_back(a.data(),strlen(a.data()));
    return out;
}

ReaderuprootTree::ReaderuprootTree(const std::string &filename,const std::string &treename,const std::string &branchname):
    m_tree_name(treename), m_branch_name(branchname)
{
    if (!init(filename)) return;
    m_event_data = new GenEventData();
    m_run_info_data = new GenRunInfoData();
}

bool ReaderuprootTree::init(const std::string &filename)
{
    set_run_info(std::make_shared<GenRunInfo>());
    m_uproot_module = pybind11::module_::import("uproot");
    m_numpy_module = pybind11::module_::import("numpy");

    bool result = false;
    if (!m_uproot_module || !m_numpy_module ) {
      pybind11::print( "ReaderuprootTree: cannot initialize python modules uproot and/or numpy. Please check your uproot and/or numpy instalation.");
      HEPMC3_ERROR( "ReaderuprootTree: cannot initialize python modules uproot and/or numpy. Please check your uproot and/or numpy instalation.")
      return result;
    }

    m_file = m_uproot_module.attr("open")(filename);
    if (m_file) {
      m_tree = m_file[(m_tree_name + "/"+ m_branch_name).c_str()];
      m_genruninfo = m_file[(m_tree_name + "/"+ "GenRunInfo").c_str()];
    }
    if (m_tree){
       m_tree_getEntries = m_tree.attr("num_entries").cast<long int>();
    }
   if (m_tree && m_file && m_genruninfo && m_tree_getEntries) result = true;
    return result;
}

bool ReaderuprootTree::skip(const int n)
{
    m_events_count+=n;
    return m_events_count <= m_tree_getEntries;
}

bool ReaderuprootTree::read_event(GenEvent& evt)
{
    if (m_events_count >= m_tree_getEntries) {  m_events_count++; return false;}
    m_event_data->particles.clear();
    m_event_data->vertices.clear();
    m_event_data->links1.clear();
    m_event_data->links2.clear();
    m_event_data->attribute_id.clear();
    m_event_data->attribute_name.clear();
    m_event_data->attribute_string.clear();
    auto event_number_v  = get_vector<int>(m_tree, "event_number");
    if (event_number_v.empty()) { m_events_count++; return false;}
    auto weights = get_vector<double>(m_tree, "weights");
    auto event_pos_1_v = get_vector<double>(m_tree, "event_pos/event_pos.m_v1");
    if (event_pos_1_v.empty()) { m_events_count++; return false;}
    auto event_pos_2_v = get_vector<double>(m_tree,"event_pos/event_pos.m_v2");
    if (event_pos_2_v.empty()) { m_events_count++; return false;}
    auto event_pos_3_v = get_vector<double>(m_tree, "event_pos/event_pos.m_v3");
    if (event_pos_3_v.empty()) { m_events_count++; return false;}
    auto event_pos_4_v = get_vector<double>(m_tree, "event_pos/event_pos.m_v4");
    if (event_pos_4_v.empty()) { m_events_count++; return false;}
    auto momentum_unit_v = get_vector<int>(m_tree, "momentum_unit");
    if (momentum_unit_v.empty()) { m_events_count++; return false;}
    auto length_unit_v = get_vector<int>(m_tree, "length_unit");
    if (length_unit_v.empty()) { m_events_count++; return false;}

    auto event_number    = event_number_v.at(0);
    auto event_pos_1     = event_pos_1_v.at(0);
    auto event_pos_2     = event_pos_2_v.at(0);
    auto event_pos_3     = event_pos_3_v.at(0);
    auto event_pos_4     = event_pos_4_v.at(0);
    auto momentum_unit   = momentum_unit_v.at(0);
    auto length_unit     = length_unit_v.at(0);

    auto particlesmomentumm_v1    = get_vector<double>(m_tree, "particles/particles.momentum.m_v1");
    auto particlesmomentumm_v2    = get_vector<double>(m_tree, "particles/particles.momentum.m_v2");
    auto particlesmomentumm_v3    = get_vector<double>(m_tree, "particles/particles.momentum.m_v3");
    auto particlesmomentumm_v4    = get_vector<double>(m_tree, "particles/particles.momentum.m_v4");
    auto particlesmass            = get_vector<double>(m_tree, "particles/particles.mass");
    auto particlesis_mass_set     = get_vector<bool>(m_tree, "particles/particles.is_mass_set");
    auto particlesparticlespid    = get_vector<int>(m_tree, "particles/particles.pid");
    auto particlesparticlesstatus = get_vector<int>(m_tree, "particles/particles.status");

    auto verticespositionm_v1    = get_vector<double>(m_tree, "vertices/vertices.position.m_v1");
    auto verticespositionm_v2    = get_vector<double>(m_tree, "vertices/vertices.position.m_v2");
    auto verticespositionm_v3    = get_vector<double>(m_tree, "vertices/vertices.position.m_v3");
    auto verticespositionm_v4    = get_vector<double>(m_tree, "vertices/vertices.position.m_v4");
    auto verticesverticesstatus  = get_vector<int>(m_tree, "vertices/vertices.status");

    m_event_data->event_number = event_number;
    m_event_data->momentum_unit = momentum_unit == 0?HepMC3::Units::MEV:HepMC3::Units::GEV;
    m_event_data->length_unit = length_unit == 0?HepMC3::Units::MM:HepMC3::Units::CM;
    m_event_data->event_pos = HepMC3::FourVector(event_pos_1, event_pos_2, event_pos_3, event_pos_4) ;
    m_event_data->links1 = get_vector<int>(m_tree, "links1");
    m_event_data->links2 = get_vector<int>(m_tree, "links2");
    m_event_data->weights = weights;
    m_event_data->attribute_id = get_vector<int>(m_tree, "attribute_id");
    m_event_data->attribute_name = get_vector<std::string>(m_tree, "attribute_name");
    m_event_data->attribute_string = get_vector<std::string>(m_tree, "attribute_string");

    m_event_data->particles.reserve(particlesparticlespid.size());

    for (size_t k = 0; k < particlesparticlespid.size(); k++)
    {
        HepMC3::GenParticleData p = { particlesparticlespid[k], particlesparticlesstatus[k], particlesis_mass_set[k], particlesmass[k],
                                     HepMC3::FourVector(particlesmomentumm_v1[k], particlesmomentumm_v2[k], particlesmomentumm_v3[k], particlesmomentumm_v4[k])
                                    };
        m_event_data->particles.emplace_back(p);
    }
    m_event_data->particles.reserve(verticesverticesstatus.size());

    for (size_t k=0; k < verticesverticesstatus.size(); k++)
    {
        HepMC3::GenVertexData v = { verticesverticesstatus[k], HepMC3::FourVector(verticespositionm_v1[k], verticespositionm_v2[k], verticespositionm_v3[k], verticespositionm_v4[k])};
        m_event_data->vertices.emplace_back(v);
    }

    evt.read_data(*m_event_data);

    m_run_info_data->weight_names.clear();
    m_run_info_data->tool_name.clear();
    m_run_info_data->tool_version.clear();
    m_run_info_data->tool_description.clear();
    m_run_info_data->attribute_name.clear();
    m_run_info_data->attribute_string.clear();

    m_run_info_data->weight_names       =  get_vector<std::string>(m_genruninfo, "weight_names");
    m_run_info_data->tool_name          =  get_vector<std::string>(m_genruninfo, "tool_name");
    m_run_info_data->tool_version       =  get_vector<std::string>(m_genruninfo, "tool_version");
    m_run_info_data->tool_description   =  get_vector<std::string>(m_genruninfo, "tool_description");
    m_run_info_data->attribute_name     =  get_vector<std::string>(m_genruninfo, "attribute_name");
    m_run_info_data->attribute_string   =  get_vector<std::string>(m_genruninfo, "attribute_string");

    run_info()->read_data(*m_run_info_data);
    evt.set_run_info(run_info());
    m_events_count++;
    return true;
}

void ReaderuprootTree::close(){}

bool ReaderuprootTree::failed()
{
    return m_events_count > m_tree_getEntries;
}
ReaderuprootTree::~ReaderuprootTree()
{
    if (m_event_data) {delete m_event_data; m_event_data=nullptr;}
    if (m_run_info_data) {delete m_run_info_data; m_run_info_data=nullptr;}
}

} // End namespace HepMC3
