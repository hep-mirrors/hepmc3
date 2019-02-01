// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/ReaderFactory.h"
namespace HepMC3{
  
  std::shared_ptr<Reader> ReaderFactory::make_reader(const std::string &filename){
    
    std::ifstream file(filename);
    if(!file.is_open()){
      ERROR("make_reader: could not open file for testing HepMC version: "<<filename);
      return shared_ptr<Reader>(nullptr);
    }

    // loop over the available Reader creator funcs
    for(auto &c: s_creators()){
      
      file.seekg(file.beg);
      
      ReaderPtr r = c->create();
      // first check the optional Reader check from the Reader itself
      // Usually this would default to false, but complex user cases
      // can choose to implement this and will match at this point
      if(r->matchesFile(file)) {
        r->initialize(filename);
        return r;
      }
      
      if(r->matchesFile(filename)){
        r->initialize(filename);
        return r;
      }
      
      // this is the list of signatures strings to test the reader
      // e.g. ROOT files contain the signature root on the first line
      vector<string> signatures = r->fileSignatures();
      if(signatures.size()==0) continue;
      vector<string>::const_iterator sig = signatures.begin();

      bool success = true;
      // loop over lines to check for the signature strings.
      while(sig != signatures.end()){
        string line;
        std::getline(file, line);
        // skip blank lines
        if(line.empty()) continue;
        // if the line doesn't contain the sig
        if(line.find(*sig) == std::string::npos){
          success = false;
          break;
        }
        ++sig;
      }
      
      // if we had success matching the Reader strings to the file,
      // return that reader
      if(success){
        r->initialize(filename);
        return r;
      }
    }
    
    // If we get to here then none of the available readers match the file :(
    WARNING("ReaderFactory: Could not match available readers to file "<<filename);
    return shared_ptr<Reader>(nullptr);
  }
  
  vector<const ReaderFactory::ICreator*> &ReaderFactory::s_creators(){
    static vector<const ReaderFactory::ICreator*> creators;
    return creators;
  }
  
}


