// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterOSCAR1997.cc
 *  @brief Implementation of \b class WriterOSCAR1997
 *
 */
#include <sstream>
#include <cstdio>  // sprintf
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/WriterOSCAR1997.h"
namespace HepMC3
{


WriterOSCAR1997::WriterOSCAR1997(const std::string &filename,
                                 std::shared_ptr<GenRunInfo> run): m_file(filename), m_stream(&m_file) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    if ( !m_file.is_open() )
    {
        HEPMC3_ERROR("WriterOSCAR1997: could not open output file: " << filename )
    }
    else
    {
    const std::string generator_version= run_info()->tools().size()? 
     run_info()->tools().front().name+" "+run_info()->tools().front().version : "Unknown 0.0.0";
    const std::string header = "OSC1997A\nfinal_id_p_x\n"+generator_version+"\n";
        m_file.write(header.data(), header.length());
    }
}

WriterOSCAR1997::WriterOSCAR1997(std::ostream& stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_stream(&stream) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string generator_version= run_info()->tools().size()? 
     run_info()->tools().front().name+" "+run_info()->tools().front().version : "Unknown 0.0.0";
    const std::string header = "OSC1997A\nfinal_id_p_x\n"+generator_version+"\n";
    m_stream->write(header.data(), header.length());
}

WriterOSCAR1997::WriterOSCAR1997(std::shared_ptr<std::ostream> s_stream,
                                 std::shared_ptr<GenRunInfo> run): m_file(), m_shared_stream(s_stream), m_stream(s_stream.get()) {
    set_run_info(run);
    if ( !run_info() ) set_run_info(std::make_shared<GenRunInfo>());
    const std::string generator_version= run_info()->tools().size()? 
     run_info()->tools().front().name+" "+run_info()->tools().front().version : "Unknown 0.0.0";
    const std::string header = "OSC1997A\nfinal_id_p_x\n"+generator_version+"\n";
    m_stream->write(header.data(), header.length());
}


void WriterOSCAR1997::write_event(const GenEvent &evt)
{
    auto allparticles = evt.particles();
    int n_final = 0;
    for (auto part: allparticles) if (!part->end_vertex()) n_final++;

    char buf[512*512];//Note: the format is fixed, so no reason for complicatied tratment
    char* cursor = &(buf[0]);
    cursor += sprintf(cursor,"%8i %8i %e %e\n", evt.event_number(), n_final, 0.0, 0.0);
    unsigned long length = cursor - &(buf[0]);
    m_stream->write(buf, length);
    cursor = &(buf[0]);

    int counter = 0;
    for (auto part: allparticles) {
        if (part->end_vertex()) continue;
        //cursor += sprintf(cursor, "% 8i% 8i", OSCAR1997_Wrapper::status(index), OSCAR1997_Wrapper::id(index));
        FourVector p = part->momentum();
        FourVector v = part->production_vertex()? part->production_vertex()->position():evt.event_pos();
        cursor += sprintf(cursor, "%8i %8i %e %e %e %e %e %e %e %e %e\n", counter+1, part->pdg_id(), p.px(),p.py(),p.pz(),p.e(), part->generated_mass(), v.x(),
                          v.y(),v.z(),v.t());
        counter++;
        if (counter%100 == 0) {
            length = cursor - &(buf[0]);
            m_stream->write(buf, length);
            cursor = &(buf[0]);
        }
    }
    length = cursor - &(buf[0]);
    m_stream->write(buf, length);
}

void WriterOSCAR1997::close()
{
    std::ofstream* ofs = dynamic_cast<std::ofstream*>(m_stream);
    if (ofs && !ofs->is_open()) return;
    if (ofs) ofs->close();
}

bool WriterOSCAR1997::failed()
{
    return (bool)m_file.rdstate();
}

} // namespace HepMC3
