#ifndef ROOT_STREAMER_TOOL_H
#define ROOT_STREAMER_TOOL_H

#ifdef HEPMC2
#include "HepMC/GenEvent.h"
#else
#include "HepMC3/IO/IO_RootStreamer.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"
#include "Timer.h"

class RootStreamerTool : public ValidationTool {
public:
    RootStreamerTool():m_timer("Event serialization time") {}

    const std::string name()    { return "root streamer"; }
    bool  tool_modifies_event() { return false; }
    Timer* timer()              { return &m_timer; }

    void initialize() {}
    int  process(GenEvent &hepmc);
    void finalize()   {}

private:
    Timer m_timer;
};

#endif
