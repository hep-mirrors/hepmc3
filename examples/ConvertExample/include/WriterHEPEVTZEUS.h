#ifndef HEPMC3_WRITERHEPEVTZEUS_H
#define HEPMC3_WRITERHEPEVTZEUS_H
#include "HepMC3/WriterHEPEVT.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Data/GenEventData.h"
namespace HepMC3
{
class WriterHEPEVTZEUS : public  WriterHEPEVT
{
public:
    WriterHEPEVTZEUS(const std::string &filename);
    void write_hepevt_event_header();
    void write_hepevt_particle( int index, bool iflong );
};
}
#endif
