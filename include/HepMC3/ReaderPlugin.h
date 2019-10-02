// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERPLUGIN_H
#define HEPMC3_READERPLUGIN_H
/**
 *  @file  ReaderPlugin.h
 *  @brief Definition of \b class ReaderPlugin
 *
 *  @class HepMC3::ReaderPlugin
 *  @brief GenEvent I/O parsing and serialization using external plugin
 *
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"

#ifdef WIN32
#include <windows.h>
#define HEPMC3_HANDLETYPE HINSTANCE
#endif
#if defined(__linux__) || defined(__darwin__)
#include <dlfcn.h>
#define HEPMC3_HANDLETYPE void*
#endif

namespace HepMC3
{
class ReaderPlugin : public Reader
{
public:
#ifndef BINDER
/** @brief Constructor  to read from stream*/
ReaderPlugin(std::istream & stream,const std::string &libname, const std::string &newreader);
/** @brief Constructor to read from file*/
#endif
ReaderPlugin(const std::string& filename,const std::string &libname, const std::string &newreader);
    /** @brief Reading event */
    bool read_event(GenEvent& ev) {if(!m_reader) return false; return m_reader->read_event(ev);};
    /** @brief Close */
    void close(){ if(!m_reader) return; m_reader->close(); };
    /** @brief State */
    bool failed(){if(!m_reader) return true; return m_reader->failed();};
    /** @brief Destructor */
~ReaderPlugin();
private:
  std::shared_ptr<Reader> m_reader; ///< The actual reader
  HEPMC3_HANDLETYPE  dll_handle; ///< library handler
 };
}
#endif