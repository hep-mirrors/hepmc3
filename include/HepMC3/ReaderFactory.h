// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERFACTORY_H
#define HEPMC3_READERFACTORY_H

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"

#include <memory>
#include <string>
#include <sys/stat.h>
#include <string.h>

namespace HepMC3 {
/** @brief THis function will deduce the type of input file based on the name/URL and it's content and will return appropriate Reader*/
std::shared_ptr<Reader> deduce_reader(const std::string &filename)
{
    bool remote=false;
    if (filename.find("http://")!=std::string::npos) 	 remote=true;
    if (filename.find("https://")!=std::string::npos) 	 remote=true;
    if (filename.find("root://")!=std::string::npos) 	 remote=true;
    if (filename.find("gsidcap://")!=std::string::npos) remote=true;

    std::vector<std::string> head;
    if (!remote)
    {
        struct stat   buffer;
        if (stat (filename.c_str(), &buffer)!=0)
        {
            printf("Error  in deduce_reader: file does not exist: %s\n",filename.c_str());
            return std::shared_ptr<Reader> (nullptr);
        }

        std::ifstream file(filename);
        if(!file.is_open()) {
            printf("Error in deduce_reader: could not open file for testing HepMC version: %s\n",filename.c_str());
            return shared_ptr<Reader>(nullptr);
        }

        std::string line;
        size_t nonempty=0;
        while (std::getline(file, line)&&nonempty<3) {
            if (line.empty()) continue;
            nonempty++;
            head.push_back(line);
        }
        file.close();
    }
    /* To assure there are at least two elements in the vector*/
    head.push_back("");
    head.push_back("");
#ifdef HEPMC3_READERROOTTREE_H
    printf("Info in deduce_reader: Attempt ReaderRootTree for:  %s\n",filename.c_str());
    if( strncmp(head.at(0).c_str(),"root",4) == 0||remote)
        return std::shared_ptr<Reader>((Reader*) ( new ReaderRootTree(filename)));
#else
    printf("Info in deduce_reader: Will not attempt ReaderRootTree. include ReaderRootTree.h to enable ROOT support");
    if (remote)
    {
        printf("Info in deduce_reader: file is on remote filesystem, but no root support is enabled:  %s\n",filename.c_str());
        return shared_ptr<Reader>(nullptr);
    }
#endif
    printf("Info in deduce_reader: Attempt ReaderAscii for:  %s\n",filename.c_str());
    if( strncmp(head.at(0).c_str(),"HepMC::Version",14) == 0 && strncmp(head.at(1).c_str(),"HepMC::Asciiv3",14)==0 )
        return std::shared_ptr<Reader>((Reader*) ( new ReaderAscii(filename)));
    printf("Info in deduce_reader: Attempt ReaderAsciiHepMC2 for: %s\n",filename.c_str());
    if( strncmp(head.at(0).c_str(),"HepMC::Version",14) == 0 && strncmp(head.at(1).c_str(),"HepMC::IO_GenEvent",18)==0 )
        return std::shared_ptr<Reader>((Reader*) ( new ReaderAsciiHepMC2(filename)));
    printf("Info in deduce_reader: Attempt ReaderLHEF for:  %s\n",filename.c_str());
    if( strncmp(head.at(0).c_str(),"<LesHouchesEvents",17) == 0)
        return std::shared_ptr<Reader>((Reader*) ( new ReaderLHEF(filename)));
    printf("Info in deduce_reader: Attempt ReaderHEPEVT for:  %s\n",filename.c_str());
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
    if (HEPEVT) return std::shared_ptr<Reader>((Reader*) ( new ReaderHEPEVT(filename)));
    printf("Info in deduce_reader: All attempts failed for:  %s\n",filename.c_str());
    return shared_ptr<Reader>(nullptr);
}
}
#endif
