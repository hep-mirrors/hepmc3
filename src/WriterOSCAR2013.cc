// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterOSCAR2013.cc
 *  @brief Implementation of \b class WriterOSCAR2013
 *
 */
#include <sstream>
#include <cstdio>  // sprintf
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/WriterOSCAR2013.h"
namespace HepMC3
{
int tricharge(const int pid);


WriterOSCAR2013::WriterOSCAR2013(const std::string &filename,
                                 std::shared_ptr<GenRunInfo> run): m_file(filename), m_stream(&m_file) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    if ( !m_file.is_open() )
    {
        HEPMC3_ERROR("WriterOSCAR2013: could not open output file: " << filename )
    }
    else
    {
        const std::string header = format_run_info();
        m_file.write(header.data(), header.length());
    }
}

WriterOSCAR2013::WriterOSCAR2013(std::ostream& stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_stream(&stream) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string header = format_run_info();

    m_stream->write(header.data(), header.length());
}

WriterOSCAR2013::WriterOSCAR2013(std::shared_ptr<std::ostream> s_stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_shared_stream(s_stream), m_stream(s_stream.get()) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string header = format_run_info();
    m_stream->write(header.data(), header.length());
}

std::string WriterOSCAR2013::format_run_info() const {
    const std::string generator_name = run_info()->tools().size()? run_info()->tools().front().name : "Unknown";
    const std::string generator_version = run_info()->tools().size()? run_info()->tools().front().version : "0.0.0";
    char buf[512];
    snprintf(buf,512,"%s\n%s\n# %.100s-%.100s\n",
             "#!OSCAR2013 particle_lists t x y z mass p0 px py pz pdg ID charge",
             "# Units: fm fm fm fm GeV GeV GeV GeV GeV none none e",
             generator_name.c_str(), generator_version.c_str()
            );

    const std::string header(buf);
    return header;
}
void WriterOSCAR2013::write_event(const GenEvent &evt)
{
    auto allparticles = evt.particles();
    int n_final = 0;
    for (const auto& part: allparticles) if (!part->end_vertex()) n_final++;
    char buf[512*512];//Note: the format is fixed, so no reason for complicatied tratment
    char* cursor = &(buf[0]);
    auto hi = evt.heavy_ion();
    double v_impact_parameter = hi? hi->impact_parameter : 0.0;
    double v_rotation = hi? hi->event_plane_angle : 0.0;
    cursor += sprintf(cursor,"# event %i out %i\n", evt.event_number(), n_final);
    unsigned long length = cursor - &(buf[0]);
    m_stream->write(buf, length);
    cursor = &(buf[0]);
    const double to_fm = (evt.length_unit() == Units::MM) ? 1e+12 : 1e+13;
    const double to_gev = (evt.momentum_unit() == Units::GEV) ? 1 : 1e-3;
    int counter = 0;
    for (const auto& part: allparticles) {
        if (part->end_vertex()) continue;
        FourVector p = part->momentum();
        FourVector v = part->production_vertex() ? part->production_vertex()->position() : evt.event_pos();
        /// The format is t x y z mass p0 px py pz pdg ID charge
        cursor += sprintf(cursor, "%.0f %6.4f %6.4f %6.4f %3.3f %9.9f %9.9f %9.10f %9.9f %i %i %i\n",
                          to_fm*v.t(), to_fm*v.x(), to_fm*v.y(), to_fm*v.z(),
                          to_gev*part->generated_mass(), to_gev*p.e(), to_gev*p.px(), to_gev*p.py(), to_gev*p.pz(),
                          part->pdg_id(), part->id(),  charge(part->pdg_id())
                         );
        counter++;
        if (counter%100 == 0) {
            length = cursor - &(buf[0]);
            m_stream->write(buf, length);
            cursor = &(buf[0]);
        }
    }
    /// End of event. Note: scattering_projectile_target is not known.
    cursor += sprintf(cursor,"# event %i end 0 impact  %6.3f scattering_projectile_target yes\n", evt.event_number(), v_impact_parameter);
    length = cursor - &(buf[0]);
    m_stream->write(buf, length);
}

int WriterOSCAR2013::charge(const int pdg) const {
    if (m_pdg_to_charge.count(pdg)) {
        return  m_pdg_to_charge.at(pdg);
    } else {
        int res = tricharge(pdg)/3;
        m_pdg_to_charge[pdg] = res;
        return res;
    }
}

void WriterOSCAR2013::close()
{
    std::ofstream* ofs = dynamic_cast<std::ofstream*>(m_stream);
    if (ofs && !ofs->is_open()) return;
    if (ofs) ofs->close();
}

bool WriterOSCAR2013::failed()
{
    return (bool)m_file.rdstate();
}

} // namespace HepMC3
