// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenRunInfo.cc
 *  @brief Implementation of \b class GenRunInfo
 *
 */
#include <sstream>

#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/GenRunInfo.h"


namespace HepMC3 {


void GenRunInfo::set_weight_names(const std::vector<std::string> & names) {
    m_weight_indices.clear();
    m_weight_names = names;
    for ( int i = 0, N = names.size(); i < N; ++i ) {
        std::string name = names[i];
        if ( name.empty() ) {
            std::ostringstream oss;
            oss << i;
            name = oss.str();
            m_weight_names[i] = name;
        }
        if ( has_weight(name) ) {
            throw std::logic_error("GenRunInfo::set_weight_names: "
                                   "Duplicate weight name '" + name +
                                   "' found.");
        }
        m_weight_indices[name] = i;
    }
}

std::string GenRunInfo::attribute_as_string(const std::string &name) const {
    std::lock_guard<std::recursive_mutex> lock(m_lock_attributes);
    auto i = m_attributes.find(name);
    if ( i == m_attributes.end() ) return {};

    if ( !i->second ) return {};

    std::string ret;
    i->second->to_string(ret);

    return ret;
}

void GenRunInfo::write_data(GenRunInfoData& data) const {
    // Weight names
    data.weight_names = this->weight_names();

    // Attributes
    using att_val_t = std::map<std::string, std::shared_ptr<Attribute>>::value_type;

    for (const att_val_t& vt: m_attributes) {
        std::string att;
        vt.second->to_string(att);

        data.attribute_name.  emplace_back(vt.first);
        data.attribute_string.emplace_back(att);
    }

    // Tools
    for ( const ToolInfo &tool: this->tools() ) {
        data.tool_name.       emplace_back(tool.name);
        data.tool_version.    emplace_back(tool.version);
        data.tool_description.emplace_back(tool.description);
    }
}


std::vector<std::string> GenRunInfo::attribute_names() const {
    std::vector<std::string> results;
    for (const auto& vt1: m_attributes) {
        results.emplace_back(vt1.first);
    }
    return results;
}

void GenRunInfo::read_data(const GenRunInfoData& data) {
    // Weight names
    set_weight_names(data.weight_names);

    // Attributes
    for (unsigned int i = 0; i < data.attribute_name.size(); ++i) {
        add_attribute(data.attribute_name[i],
                      std::make_shared<StringAttribute>(data.attribute_string[i]));
    }

    // Tools
    for (unsigned int i = 0; i < data.tool_name.size(); ++i) {
        ToolInfo ti;
        ti.name        = data.tool_name[i];
        ti.version     = data.tool_version[i];
        ti.description = data.tool_description[i];

        this->tools().emplace_back(ti);
    }
}

GenRunInfo::GenRunInfo(const GenRunInfo& r)
{
    if (this != &r)
    {
        std::lock(m_lock_attributes, r.m_lock_attributes);
        std::lock_guard<std::recursive_mutex> lhs_lk(m_lock_attributes, std::adopt_lock);
        std::lock_guard<std::recursive_mutex> rhs_lk(r.m_lock_attributes, std::adopt_lock);
        GenRunInfoData tdata;
        r.write_data(tdata);
        read_data(tdata);
    }
}
GenRunInfo& GenRunInfo::operator=(const GenRunInfo& r)
{
    if (this != &r)
    {
        std::lock(m_lock_attributes, r.m_lock_attributes);
        std::lock_guard<std::recursive_mutex> lhs_lk(m_lock_attributes, std::adopt_lock);
        std::lock_guard<std::recursive_mutex> rhs_lk(r.m_lock_attributes, std::adopt_lock);
        GenRunInfoData tdata;
        r.write_data(tdata);
        read_data(tdata);
    }
    return *this;
}

} // namespace HepMC3
