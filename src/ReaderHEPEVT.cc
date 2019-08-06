// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file ReaderHEPEVT.cc
 *  @brief Implementation of \b class ReaderHEPEVT
 *
 */
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/HEPEVT_Wrapper.h"

#include <sstream>
namespace HepMC3
{

ReaderHEPEVT::ReaderHEPEVT(const std::string &filename)
 : m_file(filename), m_stream(0), m_isstream(false), m_vertices_positions_present(true)
{
    if( !m_file.is_open() ) {
        ERROR( "ReaderHEPEVT: could not open input file: "<<filename )
    }
    else
        {
            set_run_info(make_shared<GenRunInfo>());
            hepevtbuffer=(char*)(new struct HEPEVT());
            HEPEVT_Wrapper::set_hepevt_address(hepevtbuffer);
        }
}

ReaderHEPEVT::ReaderHEPEVT(std::istream & stream)
 : m_stream(&stream), m_isstream(true), m_vertices_positions_present(true)
{
    if( !m_stream->good() ) {
        ERROR( "ReaderHEPEVT: could not open input stream  ")
    }
    else
        {
            set_run_info(make_shared<GenRunInfo>());
            hepevtbuffer=(char*)(new struct HEPEVT());
            HEPEVT_Wrapper::set_hepevt_address(hepevtbuffer);
        }
}

bool ReaderHEPEVT::read_hepevt_event_header()
{
    const size_t       max_e_buffer_size=512;
    char buf_e[max_e_buffer_size];
    bool eventline=false;
    int m_i=0, m_p=0;
    while(!eventline)
        {
            m_isstream ? m_stream->getline(buf_e,max_e_buffer_size) : m_file.getline(buf_e,max_e_buffer_size);
            if( strlen(buf_e) == 0 ) return false;
            std::stringstream st_e(buf_e);
            char attr=' ';
            eventline=false;
            while (!eventline)
                {
                    if (!(st_e>>attr)) break;
                    if (attr==' ') continue;
                    else eventline=false;
                    if (attr=='E')
                        {
                            eventline=static_cast<bool>(st_e>>m_i>>m_p);
                        }
                }
        }
    HEPEVT_Wrapper::set_event_number(m_i);
    HEPEVT_Wrapper::set_number_entries(m_p);
    return eventline;
}


bool ReaderHEPEVT::read_hepevt_particle( int i, bool iflong )
{
	const size_t       max_p_buffer_size=512;
	const size_t       max_v_buffer_size=512;
    char buf_p[max_p_buffer_size];
    char buf_v[max_v_buffer_size];
    int   intcodes[6];
    double fltcodes1[5];
    double fltcodes2[4];
    m_isstream ? m_stream->getline(buf_p,max_p_buffer_size) : m_file.getline(buf_p,max_p_buffer_size);
    if( strlen(buf_p) == 0 ) return false;    
    if (iflong) {
            m_isstream ? m_stream->getline(buf_v,max_v_buffer_size) : m_file.getline(buf_v,max_v_buffer_size);
            if( strlen(buf_v) == 0 ) return false;        
    }
    std::stringstream st_p(buf_p);
    std::stringstream st_v(buf_v);
    if (iflong)
        {
            if (!static_cast<bool>(st_p>>intcodes[0]>>intcodes[1]>>intcodes[2]>>intcodes[3]>>intcodes[4]>>intcodes[5]>>fltcodes1[0]>>fltcodes1[1]>>fltcodes1[2]>>fltcodes1[3]>>fltcodes1[4])) { ERROR( "ReaderHEPEVT: Error reading particle momenta");     return false;}
            if (!static_cast<bool>(st_v>>fltcodes2[0]>>fltcodes2[1]>>fltcodes2[2]>>fltcodes2[3])) { ERROR( "ReaderHEPEVT: Error reading particle vertex");  return false;}
        }
    else
        {
            if (!static_cast<bool>(st_p>>intcodes[0]>>intcodes[1]>>intcodes[4]>>intcodes[5]>>fltcodes1[0]>>fltcodes1[1]>>fltcodes1[2]>>fltcodes1[4])) {ERROR( "ReaderHEPEVT: Error reading particle momenta");     return false;}
            intcodes[2]=0;//FIXME!
            intcodes[3]=0;//FIXME!
            fltcodes1[3]=std::sqrt(fltcodes1[0]*fltcodes1[0]+fltcodes1[1]*fltcodes1[1]+fltcodes1[2]*fltcodes1[2]+fltcodes1[4]*fltcodes1[4]);
            fltcodes2[0]=0;
            fltcodes2[1]=0;
            fltcodes2[2]=0;
            fltcodes2[3]=0;
        }
    HEPEVT_Wrapper::set_status(i,intcodes[0]);
    HEPEVT_Wrapper::set_id(i,intcodes[1]);
    HEPEVT_Wrapper::set_parents(i,intcodes[2],std::max(intcodes[2],intcodes[3]));/* Pythia writes second mother 0*/
    HEPEVT_Wrapper::set_children(i,intcodes[4],intcodes[5]);
    HEPEVT_Wrapper::set_momentum(i,fltcodes1[0],fltcodes1[1],fltcodes1[2],fltcodes1[3]);
    HEPEVT_Wrapper::set_mass(i,fltcodes1[4]);
    HEPEVT_Wrapper::set_position(i,fltcodes2[0],fltcodes2[1],fltcodes2[2],fltcodes2[3]);
    return true;

}

bool ReaderHEPEVT::read_event(GenEvent& evt, bool iflong)
{
    evt.clear();
    HEPEVT_Wrapper::zero_everything();
    bool fileok=read_hepevt_event_header();
    for (int i=1; (i<=HEPEVT_Wrapper::number_entries())&&fileok; i++)
        fileok=read_hepevt_particle(i, iflong);
    bool result=false;
    if (fileok)
        {
            result=HEPEVT_Wrapper::HEPEVT_to_GenEvent(&evt);
            shared_ptr<GenRunInfo> g=make_shared<GenRunInfo>();
            std::vector<std::string> weightnames;
            weightnames.push_back("0");
            std::vector<double> wts;
            wts.push_back(1.0);
            g->set_weight_names(weightnames);
            evt.set_run_info(g);
            evt.weights()=wts;
        }
    else 
       {
        m_isstream ? m_stream->clear(ios::badbit) : m_file.clear(ios::badbit);
       }
    return result;
}
bool ReaderHEPEVT::read_event(GenEvent& evt)
{
if ( (!m_file.is_open()) && (!m_isstream) ) return false;
return read_event(evt,m_vertices_positions_present); 
}


void ReaderHEPEVT::close()
{ 
   if (hepevtbuffer) delete hepevtbuffer;
   if( !m_file.is_open()) return;
   m_file.close();   
}

bool ReaderHEPEVT::failed()
{
   return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate();
}

} // namespace HepMC3
