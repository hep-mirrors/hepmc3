#ifndef  HEPMC3_IO_HEPMC2_ADAPTER_H
#define  HEPMC3_IO_HEPMC2_ADAPTER_H
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
    IO_HepMC2_adapter(const std::string& filename):IO_Base(filename,std::ios::in) {}

//
// Functions
//
public:
    /// Writes HepMC3::GenEvent in HepMC2 IO file format
    /// NOT IMPLEMENTED!
    void write_event(const GenEvent *evt);

    /// Reads HepMC2 event from IO file and converts it to HepMC3::GenEvent
    bool fill_next_event(GenEvent *evt);

private:
    int parse_event_information   (GenEvent  *evt, const char *buf);
    int parse_vertex_information  (GenVertex   *v, const char *buf);
    int parse_particle_information(GenParticle *p, const char *buf);
};

} // namespace HepMC3

#endif
