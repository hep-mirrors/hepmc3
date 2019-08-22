// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERGZ_H
#define HEPMC3_READERGZ_H
///
/// @file  ReaderGZ.h
/// @brief Definition of class \b ReaderGZ
///
/// @class HepMC3::ReaderGZ
/// @brief GenEvent I/O parsing for structured text files compressed with gzip
///
/// @ingroup IO
///
#include <string>
#include <fstream>
#include <istream>
#include <string.h>
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#include "HepMC3/GenEvent.h"
#include "gzstream.h"
namespace HepMC3 {
/** @brief Union to hold first 4 byts of file, i.e. magic bytes */
union magic_t {
    uint8_t bytes[4]; ///< bytes
    uint32_t number;  ///< int
};
class ReaderGZ : public Reader {
public:
    /** @brief Construcor*/
    ReaderGZ(const std::string& filename) : m_gzstream(filename.c_str()), m_gzstream_test(filename.c_str())
    {
        std::ifstream file(filename);
        if(!file.is_open()) {
            printf("Error in ReaderGZ: could not open file%s\n",filename.c_str());
            return;
        }
        magic_t my_magic = {0x1f, 0x8b, 0x08, 0x08};
        magic_t file_magic;
        file.read((char *) file_magic.bytes, sizeof(file_magic));
        if ( file_magic.number == my_magic.number )
        {
            m_reader=deduce_reader(m_gzstream_test,m_gzstream);
        }
        else
        {
            printf("Error in ReaderGZ: make sure %s is a gziped file!\n",filename.c_str());
            return;
        }
    };
    ~ReaderGZ() {};
    /** @brief Read event */
    bool read_event(GenEvent& evt) {
        return m_reader->read_event(evt);
    };
    /** @brief State */
    bool failed() {
        return m_gzstream.rdstate();
    }
    /** @brief Close  */
    void close()  {
        if (m_reader) m_reader->close();
    };
private:
    igzstream   m_gzstream;  ///< Stream to read
    igzstream   m_gzstream_test; ///< Stream to test
    std::shared_ptr<Reader>      m_reader; ///< Actual reader
    /** @brief THis function will deduce the type of input file based on the name/URL and it's content and will return appropriate Reader*/
    std::shared_ptr<Reader> deduce_reader(std::istream & stream_test,std::istream & stream) {
        std::vector<std::string> head;
        head.push_back(std::string(""));
        head.push_back(std::string(""));
        int goodcount=0;
        int count=0;
        while (getline(stream_test,head[goodcount])&&goodcount<1&&count<10) {
            if (head[goodcount].length()) goodcount++;
            count++;
        }
        if (count>10&&goodcount<2)
        {
            printf("Info in ReaderGZ::deduce_reader: The first %i lines in stream do not contain readable information\n",count);
            return shared_ptr<Reader>(nullptr);
        }
        printf("Info in ReaderGZ::deduce_reader: Attempt ReaderAscii for stream\n");
        if( strncmp(head.at(0).c_str(),"HepMC::Version",14) == 0 && strncmp(head.at(1).c_str(),"HepMC::Asciiv3",14)==0 )
            return std::shared_ptr<Reader>((Reader*) ( new ReaderAscii(stream)));
        printf("Info in ReaderGZ::deduce_reader: Attempt ReaderAsciiHepMC2 for stream\n");
        if( strncmp(head.at(0).c_str(),"HepMC::Version",14) == 0 && strncmp(head.at(1).c_str(),"HepMC::IO_GenEvent",18)==0 )
            return std::shared_ptr<Reader>((Reader*) ( new ReaderAsciiHepMC2(stream)));
        printf("Info in deduce_reader: Attempt ReaderLHEF for stream\n");
        if( strncmp(head.at(0).c_str(),"<LesHouchesEvents",17) == 0)
            return std::shared_ptr<Reader>((Reader*) ( new ReaderLHEF(stream)));
        printf("Info in deduce_reader: Attempt ReaderHEPEVT for stream\n");
        std::stringstream st_e(head.at(0).c_str());
        char attr=' ';
        bool HEPEVT=true;
        int m_i,m_p;
        while (true)
        {
            if (!(st_e>>attr)) {
                HEPEVT=false;
                break;
            }
            if (attr==' ') continue;
            if (attr!='E') {
                HEPEVT=false;
                break;
            }
            HEPEVT=static_cast<bool>(st_e>>m_i>>m_p);
            break;
        }
        if (HEPEVT)     return std::shared_ptr<Reader>((Reader*) ( new ReaderHEPEVT(stream)));
        printf("Info in deduce_reader: All attempts failed for stream\n");
        return shared_ptr<Reader>(nullptr);
    }
};
}
#endif
