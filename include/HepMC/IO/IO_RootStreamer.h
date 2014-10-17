#ifndef  HEPMC_IO_ROOT_STREAMER_H
#define  HEPMC_IO_ROOT_STREAMER_H
/**
 *  @file  IO_RootStreamer.h
 *  @brief Definition of \b class IO_RootStreamer
 *
 *  @class HepMC::IO_RootStreamer
 *  @brief GenEvent I/O parsing and serialization for root files
 *
 *  @ingroup IO
 *
 */
#include "HepMC/IO/IO_Base.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Data/GenEventData.h"

namespace HepMC {


class IO_RootStreamer : public IO_Base {
//
// Constructors
//
public:
    /** Default constructor */
    IO_RootStreamer();

//
// Functions
//
public:
    /** @brief Write event to file
     *
     *  @param[in] evt Event to be serialized
     */
    void write_event(const GenEvent &evt);

    /** @brief Load event from file
     *
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent &evt);

//
// Accessors
//
public:
    GenEventData& data() { return m_data; } // Get event data

//
// Fields
//
private:
    GenEventData m_data; // Event data
};

} // namespace HepMC

#endif
