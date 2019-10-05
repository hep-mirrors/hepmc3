// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file ReaderPlugin.cc
/// @brief Implementation of \b class ReaderPlugin
///
#include "HepMC3/ReaderPlugin.h"
#include "HepMC3/GenEvent.h"
#include <cstring>
#include <sstream>

namespace HepMC3 {

ReaderPlugin::ReaderPlugin(std::istream & stream,const std::string &libname, const std::string &newreader) {

#ifdef WIN32
    dll_handle = LoadLibrary(libname.c_str());
    if (!dll_handle) { printf("Error  while loading library %s\n",libname.c_str()); m_reader=nullptr; return;  }
    typedef Reader* (__stdcall *f_funci)(std::istream & stream);
    f_funci newReader = (f_funci)GetProcAddress(dll_handle, newreader.c_str());
    if (!newReader) { printf("Error  while loading function %s from  library %s\n",newreader.c_str(),libname.c_str()); m_reader=nullptr; return;  }
    m_reader=(Reader*)(newReader(stream));
#endif

#if defined(__linux__) || defined(__darwin__)
    dll_handle=nullptr;
    dll_handle = dlopen(libname.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!dll_handle) { printf("Error  while loading library %s: %s\n",libname.c_str(),dlerror()); m_reader=nullptr; return;  }
    Reader* (*newReader)(std::istream & stream);
    newReader=(Reader* (*)(std::istream & stream))dlsym(dll_handle, newreader.c_str());
    if (!newReader) { printf("Error  while loading function %s from  library %s: %s\n",newreader.c_str(),libname.c_str(),dlerror()); m_reader=nullptr; return;   }
    m_reader=(Reader*)(newReader(stream));
#endif

}
/** @brief Constructor */
ReaderPlugin::ReaderPlugin(const std::string& filename,const std::string &libname, const std::string &newreader) {

#ifdef WIN32
    dll_handle = LoadLibrary(libname.c_str());
    if (!dll_handle) { printf("Error  while loading library %s\n",libname.c_str()); m_reader=nullptr; return;  }
    typedef Reader* (__stdcall *f_funci)(const std::string&);
    f_funci newReader = (f_funci)GetProcAddress(dll_handle, newreader.c_str());
    if (!newReader) { printf("Error  while loading function %s from  library %s\n",newreader.c_str(),libname.c_str()); m_reader=nullptr; return;  }
    m_reader=(Reader*)(newReader(filename));
#endif

#if defined(__linux__) || defined(__darwin__)
    dll_handle=nullptr;
    dll_handle = dlopen(libname.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!dll_handle) { printf("Error  while loading library %s: %s\n",libname.c_str(),dlerror()); m_reader=nullptr; return;  }
    Reader* (*newReader)(const std::string&);
    newReader=(Reader* (*)(const std::string&))dlsym(dll_handle, newreader.c_str());
    if (!newReader) { printf("Error  while loading function %s from  library %s: %s\n",newreader.c_str(),libname.c_str(),dlerror()); m_reader=nullptr; return;   }
    m_reader=(Reader*)(newReader(filename));
#endif
}
ReaderPlugin::~ReaderPlugin() {
    if (m_reader) m_reader->close();
    if (m_reader) delete m_reader;
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
