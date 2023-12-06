// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READER_ASCII_HEPMC2_H
#define HEPMC3_READER_ASCII_HEPMC2_H
/**
 *  @file  ReaderAsciiHepMC2.h
 *  @brief Definition of \b class ReaderAsciiHepMC2
 *
 *  @class HepMC3::ReaderAsciiHepMC2
 *  @brief Parser for HepMC2 I/O files
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/Reader.h"

#include "HepMC3/GenEvent.h"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <fstream>
#include <istream>

namespace HepMC3 {



class ReaderAsciiHepMC2 : public Reader {
//
// Constructors
//
public:
    /** @brief Default constructor */
    ReaderAsciiHepMC2(const std::string& filename);

    /// The ctor to read from stream
    ReaderAsciiHepMC2(std::istream &);

    /// The ctor to read from temp stream
    ReaderAsciiHepMC2(std::shared_ptr<std::istream> s_stream);

    /// @brief Destructor
    ~ReaderAsciiHepMC2();
//
// Functions
//

    /// @brief skip events
    bool skip(const int)  override;

    /** @brief Implementation of Reader::read_event */
    bool read_event(GenEvent &evt)  override;

    /// @brief Return status of the stream
    bool failed()  override;

    /// @brief Close file stream
    void close()  override;

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
    int parse_vertex_information(GenEvent& evt, const char *buf);

    /** @brief Parse particle
     *
     *  Helper routine for parsing single particle information
     *  @param[in] buf Line of text that needs to be parsed
     */
    int parse_particle_information(GenEvent& evt, const char *buf);

    /** @brief Parse weight names
     *
     *  Helper routine for parsing weight names
     *  @param[in] buf Line of text that needs to be parsed
     */
    bool parse_weight_names(const char *buf);

    /** @brief Parse heavy ion information
     *
     *  Helper routine for parsing heavy ion information
     *  @param[out] evt Event that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    bool parse_heavy_ion(GenEvent &evt, const char *buf);

    /** @brief Parse pdf information
     *
     *  Helper routine for parsing pdf information
     *  @param[out] evt Event that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    bool parse_pdf_info(GenEvent &evt, const char *buf);


    /** @brief Parse pdf information
    *
    *  Helper routine for parsing cross-section information
    *  @param[out] evt Event that will be filled with new data
    *  @param[in]  buf Line of text that needs to be parsed
    */
    bool parse_xs_info(GenEvent &evt, const char *buf);



//
// Fields
//
private:
    std::ifstream m_file; //!< Input file
    std::shared_ptr<std::istream> m_shared_stream; ///< For ctor when reading from temp stream
    std::istream* m_stream; ///< For ctor when reading from stream
    bool m_isstream; ///< toggles usage of m_file or m_stream


    /** @brief Temp storage for  prod vertex ids */
    std::map<GenParticlePtr, int >  m_forward_mothers;
    /** @brief Temp mapping from ID to vertex */
    std::unordered_map<int,GenVertexPtr> m_vertices;
    /** @brief Temp mapping from ID to particle */
    std::unordered_map<int,GenParticlePtr> m_particles;
    /** Last read vertex */
    GenVertexPtr m_last_vertex;
};

} // namespace HepMC3

#endif
