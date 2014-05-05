#ifndef  HEPMC3_IO_HEPMC2_ADAPTER_H
#define  HEPMC3_IO_HEPMC2_ADAPTER_H
/**
 *  @file  IO_HepMC2_adapter.h
 *  @brief Definition of \b class HepMC3::IO_HepMC2_adapter
 *
 *  @class HepMC3::IO_HepMC2_adapter
 *  @brief Parser for HepMC2 I/O files
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/IO/IO_Base.h"

#include <string>
#include <fstream>

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
    int parse_event_information(GenEvent &evt, const char *buf);

    /** Parse vertex
     *  Helper routine for parsing single event information
     *  @param[in] buf Line of text that needs to be parsed
     */
    int parse_vertex_information(const char *buf);

    /** Parse particle
     *  Helper routine for parsing single particle information
     *  @param[in] buf Line of text that needs to be parsed
     */
    int parse_particle_information(const char *buf);

//
// Fields
//
private:
    vector< std::pair< int,GenVertex >   > m_vertex_cache;   //!< Pairs: (old barcode,Vertex)
    vector< std::pair< int,GenParticle > > m_particle_cache; //!< Pairs: (end vertex,Particle)
};

} // namespace HepMC3

#endif
