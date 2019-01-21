// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2018 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file CoreRelatives.h
/// @brief Defines basic helper classes to extract parents and children of vertex and particle
///
/// These Core versions can be used internally by the "Core" part of the HepMC I/O library without
/// risk of complication.  They are wrapped by the Search/Relatives interface for easy public use
/// The Search interface is not used by the Core library, but it does use these two basic classes.
/// These would not be used by a user
///
#ifndef HEPMC_CORE_RELATIVES_H
#define HEPMC_CORE_RELATIVES_H

#include "HepMC/GenParticle_fwd.h"
#include "HepMC/GenVertex_fwd.h"

namespace HepMC{
  
  /** @brief Provides operator to find the parent particles of a Vertex or Particle
   *
   * Note you would usually not instantiate this directly, but wrap it in a RelativesInterface
   */
  class _parents{
    
  public:
    
    template<typename GenObject_type, typename dummy>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const;
    
    template<typename GenObject_type, typename std::enable_if<std::is_same<GenVertex, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return input->particles_in();}
    
    template<typename GenObject_type, typename std::enable_if<std::is_same<GenParticle, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return (*this)(vertex(input));}
    
    template<typename GenObject_type>
    GenVertex_type<GenObject_type> vertex(GenObject_type input) const {return input->production_vertex();}
    
  };
  
  /** @brief Provides operator to find the child particles of a Vertex or Particle
   *
   * Note you would usually not instantiate this directly, but wrap it in a RelativesInterface
   */
  class _children{
    
  public:
    
    template<typename GenObject_type, typename dummy>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const;
    
    template<typename GenObject_type, typename std::enable_if<std::is_same<GenVertex, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return input->particles_out();}
    
    template<typename GenObject_type, typename std::enable_if<std::is_same<GenParticle, typename std::remove_const<typename GenObject_type::element_type>::type>::value, int*>::type = nullptr>
    GenParticles_type<GenObject_type> operator()(GenObject_type input) const {return (*this)(vertex(input));}
    
    template<typename GenObject_type>
    GenVertex_type<GenObject_type> vertex(GenObject_type input) const {return input->end_vertex();}
    
  };
  
}

#endif
