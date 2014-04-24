#ifndef VALIDATION_CONTROL_H
#define VALIDATION_CONTROL_H

#ifdef HEPMC2
#include "HepMC/GenEvent.h"
#else
#include "HepMC3/GenEvent.h"
#include "HepMC3/Search/FindParticles.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"

#include <vector>

class ValidationControl {
//
// Constructors
//
public:
    ValidationControl();
    ~ValidationControl();

//
// Functions
//
public:
    void read_file(const std::string &filename);
    bool new_event();
    void initialize();
    void process(GenEvent &hepmc);
    void finalize();
//
// Accessors
//
public:
    const std::vector<ValidationTool*>& toolchain() { return m_toolchain; }
    int   event_limit()                             { return m_events;    }
    void  set_event_limit(int events)               { m_events = events;  }

    void  print_events(int events)              { m_print_events          = events; }
    void  check_momentum_for_events(int events) { m_momentum_check_events = events; }

//
// Fields
//
private:
    std::vector<ValidationTool*> m_toolchain;

    int m_events;
    int m_momentum_check_events;
    int m_print_events;
    int m_event_counter;
    int m_status;

    bool m_has_input_source;
};

#endif