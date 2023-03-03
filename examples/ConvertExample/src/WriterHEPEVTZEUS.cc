// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "WriterHEPEVTZEUS.h"
#include "HepMC3/HEPEVT_Wrapper.h"
#include <array>
namespace HepMC3
{
WriterHEPEVTZEUS::WriterHEPEVTZEUS(const std::string &filename):WriterHEPEVT(filename) {}
void WriterHEPEVTZEUS::write_hepevt_event_header()
{
    std::array<char,512> buf;//Note: the format is fixed, so no reason for complicatied tratment
    char* cursor = buf.data();
    cursor += sprintf(cursor, " E % 12i% 12i% 12i\n", m_hepevt_interface.event_number(), 0, m_hepevt_interface.number_entries());
    m_stream->write( buf.data(), cursor - buf.data());
}
void WriterHEPEVTZEUS::write_hepevt_particle( int index, bool iflong)
{
    if (!iflong) printf("INFO: the parameter is ignored as HEPEVTZEUS always uses long format\n");
    std::array<char,512> buf;//Note: the format is fixed, so no reason for complicatied tratment
    char* cursor = buf.data();
    cursor += sprintf(cursor,"% 12i% 8i", m_hepevt_interface.status(index), m_hepevt_interface.id(index));
    cursor += sprintf(cursor,"% 8i% 8i", m_hepevt_interface.first_parent(index), m_hepevt_interface.last_parent(index));
    cursor += sprintf(cursor,"% 8i% 8i", m_hepevt_interface.first_child(index), m_hepevt_interface.last_child(index));
    cursor += sprintf(cursor,      "% 19.11E% 19.11E% 19.11E% 19.11E% 19.11E\n", m_hepevt_interface.px(index), m_hepevt_interface.py(index), m_hepevt_interface.pz(index), m_hepevt_interface.e(index), m_hepevt_interface.m(index));
    cursor += sprintf(cursor, "%-52s% 19.11E% 19.11E% 19.11E% 19.11E% 19.11E\n", " ", m_hepevt_interface.x(index), m_hepevt_interface.y(index), m_hepevt_interface.z(index), m_hepevt_interface.t(index), 0.0);
    m_stream->write( buf.data(), cursor - buf.data() );
}
}// namespace HepMC3
