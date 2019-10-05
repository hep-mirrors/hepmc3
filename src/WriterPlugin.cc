// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file WriterPlugin.cc
/// @brief Implementation of \b class WriterPlugin
///
#include "HepMC3/WriterPlugin.h"
#include "HepMC3/GenEvent.h"
#include <cstring>
#include <sstream>

namespace HepMC3 {

WriterPlugin::WriterPlugin(std::ostream & stream,const std::string &libname, const std::string &newwriter,shared_ptr<GenRunInfo> run) {

#ifdef WIN32
    dll_handle = LoadLibrary(libname.c_str());
    if (!dll_handle) { printf("Error  while loading library %s\n",libname.c_str()); m_writer=nullptr; return;  }
    typedef Writer* (__stdcall *f_funci)(std::istream & stream,shared_ptr<GenRunInfo>);
    f_funci newWriter = (f_funci)GetProcAddress(dll_handle, newwriter.c_str());
    if (!newWriter) { printf("Error  while loading function %s from  library %s\n",newwriter.c_str(),libname.c_str()); m_writer=nullptr; return;  }
    m_writer=(Writer*)(newWriter(stream,run));
#endif

#if defined(__linux__) || defined(__darwin__)
    dll_handle=nullptr;
    dll_handle = dlopen(libname.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!dll_handle) { printf("Error  while loading library %s: %s\n",libname.c_str(),dlerror()); m_writer=nullptr; return;  }
    Writer* (*newWriter)(std::ostream & stream,shared_ptr<GenRunInfo>);
    newWriter=(Writer* (*)(std::ostream & stream,shared_ptr<GenRunInfo>))dlsym(dll_handle, newwriter.c_str());
    if (!newWriter) { printf("Error  while loading function %s from  library %s: %s\n",newwriter.c_str(),libname.c_str(),dlerror()); m_writer=nullptr; return;   }
    m_writer=(Writer*)(newWriter(stream,run));
#endif


}
/** @brief Constructor */
WriterPlugin::WriterPlugin(const std::string& filename,const std::string &libname, const std::string &newwriter,shared_ptr<GenRunInfo> run) {

#ifdef WIN32
    dll_handle = LoadLibrary(libname.c_str());
    if (!dll_handle) { printf("Error  while loading library %s\n",libname.c_str()); m_writer=nullptr; return;  }
    typedef Writer* (__stdcall *f_funci)(const std::string&,shared_ptr<GenRunInfo>);
    f_funci newWriter = (f_funci)GetProcAddress(dll_handle, newwriter.c_str());
    if (!newWriter) { printf("Error  while loading function %s from  library %s\n",newwriter.c_str(),libname.c_str()); m_writer=nullptr; return;  }
    m_writer=(Writer*)(newWriter(filename,run));
#endif

#if defined(__linux__) || defined(__darwin__)
    dll_handle=nullptr;
    dll_handle = dlopen(libname.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!dll_handle) { printf("Error  while loading library %s: %s\n",libname.c_str(),dlerror()); m_writer=nullptr; return;  }
    Writer* (*newWriter)(const std::string&,shared_ptr<GenRunInfo>);
    newWriter=(Writer* (*)(const std::string&,shared_ptr<GenRunInfo>))dlsym(dll_handle, newwriter.c_str());
    if (!newWriter) { printf("Error  while loading function %s from  library %s: %s\n",newwriter.c_str(),libname.c_str(),dlerror()); m_writer=nullptr; return;   }
    m_writer=(Writer*)(newWriter(filename,run));
#endif
}

WriterPlugin::~WriterPlugin() {
    if (m_writer) m_writer->close();
    if (m_writer) delete m_writer;
#ifdef WIN32

#endif
#if defined(__linux__) || defined(__darwin__)
    if(dll_handle) {
        dlclose(dll_handle);
        dll_handle = nullptr;
    }
#endif
}
} // namespace HepMC3
