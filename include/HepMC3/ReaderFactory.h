// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_CONTRIB_READER_FACTORY_H
#define HEPMC3_CONTRIB_READER_FACTORY_H

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"

#include <memory>
#include <string>

namespace HepMC3{
  
  using ReaderPtr = std::shared_ptr<Reader>;
  
  class ReaderFactory{
  
  public:
    
  /** @brief Make a reader suitable for reading HepMC2 or HepMC 3 files, depending on which type of file is passed
   *
   *  HepMC uses different implementations of the Reader niterface for HepMC 2 and HepMC 3
   *  input files.  This means a user needs to know which kind of file they will be opening.
   *  This convenience function will attempt to discover which type the file is and return
   *  the appropriate reader.
   */
  static ReaderPtr make_reader(const std::string &filename);
    
  private:
  
    class ICreator{
      
      public:
        virtual ReaderPtr create() const = 0;
    };
    
  public:
    /** @brief Helper class to be able to construct a Reader without knowing its type
     *
     *  When defining a new file Reader type, you can register it with the ReaderFactory
     *  by a simple call to ReaderFactory::Creator<MyReaderType>(); in a single compilation block
     *  This allows additional file readers to be added dynamically at runtime simply by loading
     *  an appropriate library, which does not need to be part of HepMC.
     */
    template<class Reader_type>
    class Creator: public ICreator{
      
      public:
      
      Creator(){
        ReaderFactory::s_creators().push_back(this);
      }
      
      ReaderPtr create() const override{return make_shared<Reader_type>();}
    };
   
    /// static list of available Reader Creators accessed via func to ensure
    /// objects have been created in correct order at runtime
    static vector<const ICreator*> &s_creators();
    
  };
}

#endif
