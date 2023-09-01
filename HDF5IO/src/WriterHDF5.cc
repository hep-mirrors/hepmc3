// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterHDF5.cc
 *  @brief Implementation of \b class WriterHDF5
 *
 */
#include <array>
#include <cstdio>  // sprintf

#include "HepMC3/Version.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/IOHDF5.h"
#include "HepMC3/WriterHDF5.h"

#include <highfive/H5File.hpp>
using HighFive::File;


namespace HepMC3 {
HEPMC3_DECLARE_WRITER_FILE(WriterHDF5)

WriterHDF5::WriterHDF5(const std::string &filename, std::shared_ptr<GenRunInfo> run):
    m_events_count(0) {
    set_run_info(run);
    m_file =  new HighFive::File( filename.c_str(), HighFive::File::ReadWrite | HighFive::File::Truncate);
    if ( m_file==nullptr ) {
        HEPMC3_ERROR("WriterHDF5: problem opening file: " << filename)
        return;
    }
    auto t1 = create_compound_GenParticleData();
    t1.commit(*m_file, "GenParticleData");
    auto t2 = create_compound_GenVertexData();
    t2.commit(*m_file, "GenVertexData");
    if ( run_info() ) write_run_info();
}

void WriterHDF5::write_event(const GenEvent &evt) {
    if ( m_file==nullptr ) return;

    if ( !run_info() ) {
        set_run_info(evt.run_info());
        write_run_info();
    } else {
        if ( evt.run_info() && run_info() != evt.run_info() ) {
            HEPMC3_WARNING("WriterHDF5::write_event: GenEvents contain "
                           "different GenRunInfo objects from - only the "
                           "first such object will be serialized.")
        }
    }

    GenEventData data;
    evt.write_data(data);
    int i = m_events_count;
    m_file->createDataSet("/event"+std::to_string(i)+"/event_number", data.event_number);
    m_file->createDataSet("/event"+std::to_string(i)+"/momentum_unit", data.momentum_unit==Units::MomentumUnit::GEV?0:1);
    m_file->createDataSet("/event"+std::to_string(i)+"/length_unit", data.length_unit==Units::LengthUnit::MM?0:1);
    m_file->createDataSet("/event"+std::to_string(i)+"/vertices", data.vertices);
    m_file->createDataSet("/event"+std::to_string(i)+"/particles", data.particles);
    m_file->createDataSet("/event"+std::to_string(i)+"/weights", data.weights);
    m_file->createDataSet("/event"+std::to_string(i)+"/event_pos_0", data.event_pos.x());
    m_file->createDataSet("/event"+std::to_string(i)+"/event_pos_1", data.event_pos.y());
    m_file->createDataSet("/event"+std::to_string(i)+"/event_pos_2", data.event_pos.z());
    m_file->createDataSet("/event"+std::to_string(i)+"/event_pos_3", data.event_pos.t());
    m_file->createDataSet("/event"+std::to_string(i)+"/links1", data.links1);
    m_file->createDataSet("/event"+std::to_string(i)+"/links2", data.links2);
    if (!data.attribute_id.empty()) m_file->createDataSet("/event"+std::to_string(i)+"/attribute_id", data.attribute_id);
    if (!data.attribute_name.empty()) m_file->createDataSet("/event"+std::to_string(i)+"/attribute_name", data.attribute_name);
    if (!data.attribute_string.empty()) m_file->createDataSet("/event"+std::to_string(i)+"/attribute_string", data.attribute_string);

    ++m_events_count;
}

void WriterHDF5::write_run_info() {
    if ( m_file==nullptr ||!run_info() ) return;
    GenRunInfoData data;
    run_info()->write_data(data);
    if (!data.weight_names.empty())   m_file->createDataSet("/run_info/weight_names",data.weight_names);
    if (!data.tool_name.empty())    m_file->createDataSet("/run_info/tool_name",data.tool_name);
    if (!data.tool_version.empty()) m_file->createDataSet("/run_info/tool_version",data.tool_version);
    if (!data.tool_description.empty()) m_file->createDataSet("/run_info/tool_description",data.tool_description);
    if (!data.attribute_name.empty())   m_file->createDataSet("/run_info/attribute_name",data.attribute_name);
    if (!data.attribute_string.empty()) m_file->createDataSet("/run_info/attribute_string",data.attribute_string);
}

void WriterHDF5::close() {
    delete m_file;
    m_file=nullptr;
}

bool WriterHDF5::failed() {
    return (m_file==nullptr);
}

} // namespace HepMC3
