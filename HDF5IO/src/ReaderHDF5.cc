// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderHDF5.cc
 *  @brief Implementation of \b class ReaderHDF5
 *
 */
#include "HepMC3/Version.h"

#include "HepMC3/Version.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/Data/GenParticleData.h"
#include "HepMC3/Data/GenVertexData.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/IOHDF5.h"
#include "HepMC3/ReaderHDF5.h"
#include "HepMC3/Print.h"

#include <highfive/H5File.hpp>
using HighFive::File;


namespace HepMC3 {



HEPMC3_DECLARE_READER_FILE(ReaderHDF5)

ReaderHDF5::ReaderHDF5(const std::string &filename) {

//    if ( !m_file->IsOpen() ) {
    //      HEPMC3_ERROR("ReaderHDF5: problem opening file: " << filename)
    //    return;
    // }
    m_events_count =0;
    m_file =  new HighFive::File( filename.c_str(), HighFive::File::ReadOnly );

    GenRunInfoData rdata_read;
    if (m_file->exist("/run_info/weight_names")) m_file->getDataSet("/run_info/weight_names").read(rdata_read.weight_names);
    if (m_file->exist("/run_info/tool_name")) m_file->getDataSet("/run_info/tool_name").read(rdata_read.tool_name);
    if (m_file->exist("/run_info/tool_version"))m_file->getDataSet("/run_info/tool_version").read(rdata_read.tool_version);
    if (m_file->exist("/run_info/tool_description"))m_file->getDataSet("/run_info/tool_description").read(rdata_read.tool_description);
    if (m_file->exist("/run_info/attribute_name"))m_file->getDataSet("/run_info/attribute_name").read(rdata_read.attribute_name);
    if (m_file->exist("/run_info/attribute_string"))m_file->getDataSet("/run_info/attribute_string").read(rdata_read.attribute_string);



    std::shared_ptr<GenRunInfo> ri = std::make_shared<GenRunInfo>();


    // if (run) {
    ri->read_data(rdata_read);
    //   delete run;
    //}

    set_run_info(ri);
}

bool ReaderHDF5::skip(const int n)
{
    GenEvent evt;
    for (int nn = n; nn > 0; --nn)
    {
        if (!read_event(evt)) return false;
        evt.clear();
    }
    return !failed();
}

bool ReaderHDF5::read_event(GenEvent& evt) {
    // Skip object of different type than GenEventData
    GenEventData data;// = nullptr;


    std::string pref = "/event"+std::to_string(m_events_count)+"/";
    if (!m_file->exist(pref)) {  close();  return false;}
    m_file->getDataSet(pref+"event_number").read(data.event_number);
    int units[2];
    m_file->getDataSet(pref+"momentum_unit").read(units[0]);
    m_file->getDataSet(pref+"length_unit").read(units[1]);
    data.momentum_unit = ( units[0]==0 ? Units::MomentumUnit::GEV : Units::MomentumUnit::MEV);
    data.length_unit = ( units[1]==0 ? Units::LengthUnit::MM : Units::LengthUnit::CM);
    m_file->getDataSet(pref+"vertices").read(data.vertices);
    m_file->getDataSet(pref+"particles").read(data.particles);
    m_file->getDataSet(pref+"weights").read(data.weights);
    double v[4];
    m_file->getDataSet(pref+"event_pos_0").read(v[0]);
    m_file->getDataSet(pref+"event_pos_1").read(v[1]);
    m_file->getDataSet(pref+"event_pos_2").read(v[2]);
    m_file->getDataSet(pref+"event_pos_3").read(v[3]);
    data.event_pos = FourVector(v[0],v[1],v[2],v[3]);
    m_file->getDataSet(pref+"links1").read(data.links1);
    m_file->getDataSet(pref+"links2").read(data.links2);
    if (m_file->exist(pref+"attribute_id")) m_file->getDataSet(pref+"attribute_id").read(data.attribute_id);
    if (m_file->exist(pref+"attribute_name")) m_file->getDataSet(pref+"attribute_name").read(data.attribute_name);
    if (m_file->exist(pref+"attribute_string"))  m_file->getDataSet(pref+"attribute_string").read(data.attribute_string);

    evt.read_data(data);
    evt.set_run_info(run_info());
    //Print::content(evt);
    ++m_events_count;
    // delete data;
    return true;
}

void ReaderHDF5::close() {
    delete m_file;
    m_file=nullptr;
}

bool ReaderHDF5::failed() {
    return (m_file==nullptr);
    //  return !m_file->IsOpen();
}

} // namespace HepMC3
