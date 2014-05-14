#ifndef  HEPMC3_IO_ROOT_STREAMER_H
#define  HEPMC3_IO_ROOT_STREAMER_H
/**
 *  @file  IO_RootStreamer.h
 *  @brief Definition of \b class IO_RootStreamer
 *
 *  @class HepMC3::IO_RootStreamer
 *  @brief GenEvent I/O parsing and serialization for root files
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/IO/IO_Base.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Data/GenEventData.h"

namespace HepMC3 {


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
    GenEventData& data() { return m_data; } //!< Get event data

//
// Fields
//
private:
    GenEventData m_data; //!< Event data
};

} // namespace HepMC3

#endif
