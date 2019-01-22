// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenEventStreamer.cc
 *  @brief Implementation of \b methods GenEvent::Streamer and GenRunInfo::Streamer
 *
 */

#include "HepMC3/GenEvent.h"

#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenRunInfoData.h"

#ifdef HEPMC3_ROOTIO
#include "TBuffer.h"
#include "TClass.h"
#endif


namespace HepMC {
  
#ifdef HEPMC3_ROOTIO
  
  void GenEvent::Streamer(TBuffer &b){
    
    if (b.IsReading()){
      
      GenEventData data;
      
      b.ReadClassBuffer(TClass::GetClass("HepMC::GenEventData"), &data);
      
      read_data(data);
      
    } else {
      
      // fill the GenEventData structures
      GenEventData data;
      write_data(data);
      
      b.WriteClassBuffer(TClass::GetClass("HepMC::GenEventData"), &data);
    }
  }

  
  void GenRunInfo::Streamer(TBuffer &b){
    
    if (b.IsReading()){

      GenRunInfoData data;
      
      b.ReadClassBuffer(TClass::GetClass("HepMC::GenRunInfoData"), &data);
      
      read_data(data);
      
    } else {
      
      // fill the GenRunInfo structures
      GenRunInfoData data;
      write_data(data);
      
      b.WriteClassBuffer(TClass::GetClass("HepMC::GenRunInfoData"), &data);
    }
  }
  
#endif
  
} // namespace HepMC
