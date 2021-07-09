// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderRAW.cc
/// @brief Implementation of \b class ReaderRAW
///
#include <cstring>
#include <sstream>

#include "ReaderRAW.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Units.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenRunInfoData.h"

namespace HepMC3 {


ReaderRAW::ReaderRAW(const std::string &filename)
    : m_file(filename), m_stream(0), m_isstream(false)
{
    if ( !m_file.is_open() ) {
        HEPMC3_ERROR("ReaderRAW: could not open input file: " << filename)
    }
    set_run_info(std::make_shared<GenRunInfo>());
}

ReaderRAW::ReaderRAW(std::istream & stream)
    : m_stream(&stream), m_isstream(true)
{
    if ( !m_stream->good() ) {
        HEPMC3_ERROR("ReaderRAW: could not open input stream ")
    }
    set_run_info(std::make_shared<GenRunInfo>());
}



ReaderRAW::~ReaderRAW() { if (!m_isstream) close(); }
int deserialize(char* &input, std::vector<std::string>& ret)
{
	char* old_input=input;
        printf("HM\n");
        int n_as;
        std::vector<int> as;
        memcpy(&n_as, input, sizeof(int));
        input+=sizeof(int);
        as.resize(n_as);
        memcpy(as.data(), input, sizeof(int)*n_as);
        input+=sizeof(int)*n_as;  
        
        printf("rt1 %i %i\n", ret.size(), as.size());
        //ret(n_as);
        for (int i=0;i<n_as;i++)  { ret.push_back(std::string(input,as[i])); input+=as[i];} 
        printf("rt %i\n", ret.size());
        
        return input-old_input;
}
bool ReaderRAW::skip(const int n)
{
    int nn = n;
    char buf12[12];
    while (!failed()) {
        char  peek;
        if ( (!m_file.is_open()) && (!m_isstream) ) return false;
        m_isstream ? peek = m_stream->peek() : peek = m_file.peek();
        if ( peek == 'E' ) {
        m_isstream ? m_stream->read(buf12, 12) : m_file.read(buf12, 12);			
        int sz;
        memcpy(&sz,&(buf12[4]),sizeof(int));
        printf("Event detected, size is %i\n",sz);
        m_isstream ? m_stream->seekg(sz,ios_base::cur) : m_file.seekg(sz,ios_base::cur);	
	     nn--; }
        if ( peek == 'R' ) {
int sz;
	memcpy(&sz,&(buf12[4]),sizeof(int));
  printf("Run detected, size is %i\n",sz);
       char z[512];
       m_isstream ? m_stream->read(&(z[0]), sz) : m_file.read(&(z[0]), sz);
       char * input=&(z[0]);
  
       
       GenRunInfoData rid;
      deserialize(input, rid.weight_names);     ///< Weight names
   deserialize(input, rid.tool_name);        ///< Tool names
    deserialize(input, rid.tool_version);     ///< Tool versions
    deserialize(input, rid.tool_description); ///< Tool descriptions
    deserialize(input, rid.attribute_name);   ///< Attribute name
    deserialize(input, rid.attribute_string); ///< Attribute serialized as string
       run_info()->read_data(rid);
}
        if (nn < 0) return true;
    }
    return true;		
    
}    



bool ReaderRAW::read_event(GenEvent &evt) {
    if ( (!m_file.is_open()) && (!m_isstream) ) return false;

    const size_t       max_buffer_size = 512*512;
    char               buf[max_buffer_size];


    evt.clear();
    evt.set_run_info(run_info());
    //
    // Parse event, vertex and particle information
    //

		char buf12[12];
		bool event_context= false;
		while(!event_context)
		{
        m_isstream ? m_stream->read(buf12, 12) : m_file.read(buf12, 12);

        if ( strncmp(buf12, "HepMCBin", 8) == 0 ) {
           printf("Beginnig of file detected\n");
           /*
            if ( strncmp(buf10, "HepMCBin0000", 12) != 0 )
            {
                HEPMC3_WARNING("ReaderRAW: found unsupported expression in header. Will close the input.")
                std::cout << buf12 << std::endl;
                m_isstream ? m_stream->clear(std::ios::eofbit) : m_file.clear(std::ios::eofbit);
            }
            if (event_context) {
                is_parsing_successful = true;
                break;
            }
            continue;
          */
        m_isstream ? m_stream->read(buf12, 12) : m_file.read(buf12, 12);
        }
if ( strncmp(buf12, "R000", 4) == 0 )
{
	int sz;
	memcpy(&sz,&(buf12[4]),sizeof(int));
  printf("Run detected, size is %i\n",sz);

       char z[512];
       printf("%s<=\n", z);   
       m_isstream ? m_stream->read(&(z[0]), sz) : m_file.read(&(z[0]), sz);
       char * input=&(z[0]);
       
       printf("%s<=\n", input);   
       
       GenRunInfoData rid;
    printf("%s\n", input);   
      deserialize(input, rid.weight_names);     ///< Weight names
 printf("%s\n", input); 
   deserialize(input, rid.tool_name);        ///< Tool names
    deserialize(input, rid.tool_version);     ///< Tool versions
    deserialize(input, rid.tool_description); ///< Tool descriptions
 printf("%s\n", input); 
    deserialize(input, rid.attribute_name);   ///< Attribute name
    deserialize(input, rid.attribute_string); ///< Attribute serialized as string
       //GenRunInfoData rid;
       run_info()->read_data(rid);
        m_isstream ? m_stream->read(buf12, 12) : m_file.read(buf12, 12);

}
if ( strncmp(buf12, "E000", 4) == 0 )  event_context=true;

}

        GenEventData D;
        m_isstream ? m_stream->read((char*)&D.event_number, sizeof(int)) : m_file.read((char*)&D.event_number, sizeof(int));

D.momentum_unit = Units::GEV;
D.length_unit = Units::CM;

        int len;
        int mom;
 
        m_isstream ? m_stream->read((char*)&len, sizeof(int)) : m_file.read((char*)&len, sizeof(int));
        m_isstream ? m_stream->read((char*)&mom, sizeof(int)) : m_file.read((char*)&mom, sizeof(int));



        int n_part;
        m_isstream ? m_stream->read((char*)&n_part, sizeof(int)) : m_file.read((char*)&n_part, sizeof(int));
        D.particles.resize(n_part);
        m_isstream ? m_stream->read((char*)D.particles.data(), n_part*sizeof(GenParticleData)) : m_file.read((char*)D.particles.data(), n_part*sizeof(GenParticleData));

        int n_vert;
        m_isstream ? m_stream->read((char*)&n_vert, sizeof(int)) : m_file.read((char*)&n_vert, sizeof(int));
        D.vertices.resize(n_vert);
        m_isstream ? m_stream->read((char*)D.vertices.data(), n_vert*sizeof(GenVertexData)) : m_file.read((char*)D.vertices.data(), n_vert*sizeof(GenVertexData));


        int n_weights;
        m_isstream ? m_stream->read((char*)&n_weights, sizeof(int)) : m_file.read((char*)&n_weights, sizeof(int));
        D.weights.resize(n_weights);
        m_isstream ? m_stream->read((char*)D.weights.data(), sizeof(double)*n_weights) : m_file.read((char*)D.weights.data(), sizeof(double)*n_weights);
//printf("10->%i %i %i\n", D.event_number, D.vertices.size(), D.particles.size());
        m_isstream ? m_stream->read((char*)&D.event_pos, sizeof(FourVector)) : m_file.read((char*)&D.event_pos, sizeof(FourVector));
        
//printf("11->%i %i %f\n", D.event_number, D.weights.size(),D.weights.at(0));
        
        int n_links1;
        m_isstream ? m_stream->read((char*)&n_links1, sizeof(int)) : m_file.read((char*)&n_links1, sizeof(int));
        D.links1.resize(n_links1);
        m_isstream ? m_stream->read((char*)D.links1.data(), sizeof(int)*n_links1) : m_file.read((char*)D.links1.data(), sizeof(int)*n_links1);

        int n_links2;
        m_isstream ? m_stream->read((char*)&n_links2, sizeof(int)) : m_file.read((char*)&n_links2, sizeof(int));
        D.links2.resize(n_links2);
        m_isstream ? m_stream->read((char*)D.links2.data(), sizeof(int)*n_links2) : m_file.read((char*)D.links2.data(), sizeof(int)*n_links2);

        int n_attribute_id;
        m_isstream ? m_stream->read((char*)&n_attribute_id, sizeof(int)) : m_file.read((char*)&n_attribute_id, sizeof(int));
        D.attribute_id.resize(n_attribute_id);
        m_isstream ? m_stream->read((char*)D.attribute_id.data(), sizeof(int)*n_attribute_id) : m_file.read((char*)D.attribute_id.data(), sizeof(int)*n_attribute_id);
//printf("12->%i %i  %i %i\n", D.event_number, D.attribute_id.size(),D.links1.size(), D.links2.size() );
        int n_an;
        std::vector<int> an;
        m_isstream ? m_stream->read((char*)&n_an, sizeof(int)) : m_file.read((char*)&n_an, sizeof(int));
        an.resize(n_an);
        m_isstream ? m_stream->read((char*)an.data(), sizeof(int)*n_an) : m_file.read((char*)an.data(), sizeof(int)*n_an);
        int t_an=0;
        for (int i=0;i<n_an;i++) t_an+=an[i];
//printf("13->%i %i\n", D.event_number, t_an);

        m_isstream ? m_stream->read((char*)&buf, sizeof(char)*t_an) : m_file.read((char*)&buf, sizeof(char)*t_an);
        
        D.attribute_name.reserve(n_an);
        char* p_an=&(buf[0]);
        //for (int i=0;i<n_an;i++)  
		//	printf("14->%i   %i %i =>%s<=\n", D.event_number, i, an[i],buf);
        for (int i=0;i<n_an;i++)  { 
	//		printf("14->%i   %i %i\n", D.event_number, i, an[i]);

			D.attribute_name.push_back(std::string(p_an,an[i])); p_an+=an[i];}
//printf("23->%i %i\n", D.event_number, D.attribute_name.size());        
        
        int n_as;
        std::vector<int> as;
        m_isstream ? m_stream->read((char*)&n_as, sizeof(int)) : m_file.read((char*)&n_as, sizeof(int));
        as.resize(n_as);
        m_isstream ? m_stream->read((char*)as.data(), sizeof(int)*n_as) : m_file.read((char*)as.data(), sizeof(int)*n_as);
        int t_as=0;
        for (int i=0;i<n_as;i++) t_as+=as[i];
        m_isstream ? m_stream->read((char*)&buf, sizeof(char)*t_as) : m_file.read((char*)&buf, sizeof(char)*t_as);
        
        D.attribute_string.reserve(n_as);
        char* p_as=&(buf[0]);
        for (int i=0;i<n_as;i++)  { D.attribute_string.push_back(std::string(p_as,as[i])); p_as+=as[i];}        
   //     printf("%i\n", D.event_number);
         evt.read_data(D);

    return true;
}



bool ReaderRAW::failed() { return m_isstream ? (bool)m_stream->rdstate() :(bool)m_file.rdstate(); }

void ReaderRAW::close() {
    if ( !m_file.is_open()) return;
    m_file.close();
}


} // namespace HepMC3
