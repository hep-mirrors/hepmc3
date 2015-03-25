// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_HEPMC2_ADAPTER_H
#define  HEPMC_HEPMC2_ADAPTER_H
/**
 *  @file  HepMC2_adapter.h
 *  @brief Definition of \b class HepMC2_adapter
 *
 *  @class HepMC::HepMC2_adapter
 *  @brief Parser for HepMC2 I/O files
 *
 *  @ingroup IO
 *
 */
#include "HepMC/Reader.h"

#include "HepMC/Data/SmartPointer.h"

#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::ifstream;

namespace HepMC {

class GenEvent;

class HepMC2_adapter : public Reader {
//
// Constructors
//
public:
    /** @brief Default constructor */
    HepMC2_adapter(const std::string& filename);
//
// Functions
//
public:
    /** @brief Implementation of Reader::read_event */
    bool read_event(GenEvent &evt);

    /// @brief Return status of the stream
    ///
    /// @todo Rename!
    int rdstate() { return m_file.rdstate(); }

    /// @brief Close file stream
    void close();

private:
    /** @brief Parse event
     *
     *  Helper routine for parsing event information
     *  @param[out] evt Event that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    int parse_event_information(GenEvent &evt, const char *buf);

    /** @brief Parse units
     *
     *  Helper routine for parsing unit information
     *  @param[out] evt Event that will be filled with unit information
     *  @param[in]  buf Line of text that needs to be parsed
     */
    bool parse_units(GenEvent &evt, const char *buf);

    /** @brief Parse vertex
     *
     *  Helper routine for parsing single event information
     *  @param[in] buf Line of text that needs to be parsed
     */
    int parse_vertex_information(const char *buf);

    /** @brief Parse particle
     *
     *  Helper routine for parsing single particle information
     *  @param[in] buf Line of text that needs to be parsed
     */
    int parse_particle_information(const char *buf);

//
// Fields
//
private:
    ifstream               m_file;                //!< Input file

    vector<GenVertexPtr>   m_vertex_cache;        //!< Vertex cache
    vector<int>            m_vertex_barcodes;     //!< Old vertex barcodes

    vector<GenParticlePtr> m_particle_cache;      //!< Particle cache
    vector<int>            m_end_vertex_barcodes; //!< Old end vertex barcodes
};

} // namespace HepMC

#endif
