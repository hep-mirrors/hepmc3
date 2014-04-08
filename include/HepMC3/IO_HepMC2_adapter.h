#ifndef  HEPMC3_IO_HEPMC2_ADAPTER_H
#define  HEPMC3_IO_HEPMC2_ADAPTER_H
/**
 *  @file  IO_HepMC2_adapter.h
 *  @brief Definition of \b class HepMC3::IO_HepMC2_adapter
 *
 *  @class HepMC3::IO_HepMC2_adapter
 *  @brief Parser for HepMC2 I/O files
 *
 *  @date Created       <b> 23th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <string>
#include <fstream>
#include "HepMC3/IO_Base.h"

namespace HepMC3 {

class GenEvent;
class GenVertex;
class GenParticle;

class IO_HepMC2_adapter : public IO_Base {
//
// Constructors
//
public:
    /** Default constructor
     *  Accepts only ios::in mode
     */
    IO_HepMC2_adapter(const std::string& filename, std::ios::openmode mode = std::ios::in);

//
// Functions
//
public:
    /** Write HepMC3::GenEvent in HepMC2 IO file format
     *  @warning Function not implemented. It will be implemented if
     *           there is demand for it
     */
    void write_event(const GenEvent &evt);

    /** Parse HepMC2 event and convert it to HepMC3::GenEvent
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent &evt);

private:
    /** Parse event
     *  Helper routine for parsing event information
     *  @param[out] evt Event that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    int parse_event_information   (GenEvent  &evt, const char *buf);

    /** Parse vertex
     *  Helper routine for parsing single event information
     *  @param[out] v   Vertex that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    int parse_vertex_information  (GenVertex   *v, const char *buf);

    /** Parse particle
     *  Helper routine for parsing single particle information
     *  @param[out] p   Particle that will be filled with new data
     *  @param[in]  buf Line of text that needs to be parsed
     */
    int parse_particle_information(GenParticle *p, const char *buf);

//
// Fields
//
private:
    /** Vertex barcode cache
     *  Used to translate HepMC2 vertex barcodes to sequential barcodes
     *  of HepMC3
     */
    vector< std::pair<GenVertex*,int> >   m_vertex_barcode_cache;

    /** End vertex barcode cache
     *  Used to restore end vertex information
     */
    vector< std::pair<GenParticle*,int> > m_end_vertex_barcode_cache;
};

} // namespace HepMC3

#endif
