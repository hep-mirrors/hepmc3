// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterOSCAR1999.cc
 *  @brief Implementation of \b class WriterOSCAR1999
 *
 */
#include <sstream>
#include <cstdio>  // sprintf
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/WriterOSCAR1999.h"
namespace HepMC3
{

WriterOSCAR1999::WriterOSCAR1999(const std::string &filename,
                                 std::shared_ptr<GenRunInfo> run): m_file(filename), m_stream(&m_file) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    if ( !m_file.is_open() )
    {
        HEPMC3_ERROR("WriterOSCAR1999: could not open output file: " << filename )
    }
    else
    {
        const std::string header = format_run_info();
        m_file.write(header.data(), header.length());
    }
}

WriterOSCAR1999::WriterOSCAR1999(std::ostream& stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_stream(&stream) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string header = format_run_info();

    m_stream->write(header.data(), header.length());
}

WriterOSCAR1999::WriterOSCAR1999(std::shared_ptr<std::ostream> s_stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_shared_stream(s_stream), m_stream(s_stream.get()) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string header = format_run_info();
    m_stream->write(header.data(), header.length());
}

std::string WriterOSCAR1999::format_run_info() const {
    const std::string generator_name = run_info()->tools().size()? run_info()->tools().front().name : "Unknown";
    const std::string generator_version = run_info()->tools().size()? run_info()->tools().front().version : "0.0.0";
    char buf[512];
    snprintf(buf, 512, "%s\n%s\n# %s %s\n%s\n%s\n%s\n%s\n%s\n",
             "# OSC1999A",
             "# final_id_p_x",
             generator_name.c_str(), generator_version.c_str(),
             "# Block format:",
             "# nin nout event_number",
             "# id pdg 0 px py pz p0 mass x y z t",
             "# End of event: 0 0 event_number impact_parameter",
             "#"
            );
    const std::string header(buf);
    return header;
}
void WriterOSCAR1999::write_event(const GenEvent &evt)
{
    auto allparticles = evt.particles();
    int n_final = 0;
    for (auto part: allparticles) if (!part->end_vertex() && part->status() != 4 && part->pdg_id() != 0) n_final++;

    char buf[512*512];//Note: the format is fixed, so no reason for complicatied tratment
    char* cursor = &(buf[0]);
    auto hi = evt.heavy_ion();
    double v_impact_parameter = hi? hi->impact_parameter : 0.0;
    double v_rotation = hi? hi->event_plane_angle : 0.0;
    cursor += sprintf(cursor,"%i 0 %i\n", n_final, evt.event_number());
    unsigned long length = cursor - &(buf[0]);
    m_stream->write(buf, length);
    cursor = &(buf[0]);
    int counter = 0;
    const double to_fm = (evt.length_unit() == Units::MM) ? 1e+12 : 1e+13;
    const double to_gev = (evt.momentum_unit() == Units::GEV) ? 1 : 1e-3;
    for (auto part: allparticles) {
        if (part->end_vertex()) continue;
        if (part->status() == 4) continue;
        if (part->pdg_id() == 0) continue;
        FourVector p = part->momentum();
        FourVector v = part->production_vertex() ? part->production_vertex()->position() : evt.event_pos();
        /// Exact reproduction of the SMASH output
        ///        std::stringstream nozeros2;
        ///        nozeros2 <<       counter<< " " <<part->pdg_id() <<" 0 " <<
        ///                          p.px()<< " " <<p.py()<<" " << p.pz()<<" " << p.e()<<" " << part->generated_mass()<<" " <<
        ///                          v.x()<< " " <<v.y()<< " " <<v.z()<< " " <<int(v.t());
        ///        cursor += sprintf(cursor, "%s\n",nozeros2.str().c_str());

        cursor += snprintf(cursor, 512, "%i %i 0 %f %f %f %f %f %f %f %f %i\n",
                           counter, part->pdg_id(),
                           to_gev*p.px(), to_gev*p.py(), to_gev*p.pz(), to_gev*p.e(), to_gev*part->generated_mass(),
                           to_fm*v.x(), to_fm*v.y(), to_fm*v.z(), int(to_fm*v.t()));

        counter++;
        if (counter%100 == 0) {
            length = cursor - &(buf[0]);
            m_stream->write(buf, length);
            cursor = &(buf[0]);
        }
    }
    /// End of event
    /// What are the units of impact parameter?
    cursor += sprintf(cursor,"0 0 %i %7.3f\n", evt.event_number(), v_impact_parameter);
    length = cursor - &(buf[0]);
    m_stream->write(buf, length);
}

void WriterOSCAR1999::close()
{
    std::ofstream* ofs = dynamic_cast<std::ofstream*>(m_stream);
    if (ofs && !ofs->is_open()) return;
    if (ofs) ofs->close();
}

bool WriterOSCAR1999::failed()
{
    return (bool)m_file.rdstate();
}

} // namespace HepMC3
