// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_IO_HEPMC2_ADAPTER_H
#define  HEPMC_IO_HEPMC2_ADAPTER_H
/**
 *  @file  IO_HepMC2_adapter.h
 *  @brief Definition of \b class IO_HepMC2_adapter
 *
 *  @class HepMC::IO_HepMC2_adapter
 *  @brief Parser for HepMC2 I/O files
 *
 *  @ingroup IO
 *
 */
#include "HepMC/IO/IO_FileBase.h"

#include "HepMC/Data/SmartPointer.h"

#include <string>
#include <fstream>
#include <vector>
using std::vector;

namespace HepMC {

class GenEvent;

class IO_HepMC2_adapter : public IO_FileBase {
//
// Constructors
//
public:
    /** @brief Default constructor
     *
     *  Accepts only ios::in mode
     */
    IO_HepMC2_adapter(const std::string& filename, std::ios::openmode mode = std::ios::in);
//
// Functions
//
public:
    /** @brief Write GenEvent in HepMC2 IO file format
     *
     *  @warning Function not implemented. It will be implemented if
     *           there is demand for it
     */
    void write_event(const GenEvent &evt);

    /** @brief Parse HepMC2 event and convert it to GenEvent
     *
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent &evt);

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
    vector<GenVertexPtr>   m_vertex_cache;        //!< Vertex cache
    vector<int>            m_vertex_barcodes;     //!< Old vertex barcodes

    vector<GenParticlePtr> m_particle_cache;      //!< Particle cache
    vector<int>            m_end_vertex_barcodes; //!< Old end vertex barcodes
};

} // namespace HepMC

#endif
