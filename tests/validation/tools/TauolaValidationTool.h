#ifndef TAUOLA_VALIDATION_TOOL_H
#define TAUOLA_VALIDATION_TOOL_H

#ifdef TAUOLAPP_HEPMC2
#include "Tauola/TauolaHepMCEvent.h"
#include "HepMC/GenEvent.h"
#else
#include "Tauola/TauolaHepMC3Event.h"
#include "HepMC3/GenEvent.h"
#endif // ifdef TAUOLAPP_HEPMC2

#include "ValidationTool.h"
#include "Tauola/Tauola.h"
#include "Tauola/Log.h"

class TauolaValidationTool : public ValidationTool {
public:
    const std::string name()   { return "Tauola++"; }
    bool tool_modifies_event() { return true;       }

    void initialize() {
        Tauolapp::Tauola::initialize();
    }

    int process(GenEvent &hepmc) {

        HEPMC2CODE( Tauolapp::TauolaHepMCEvent  t_event(&hepmc); )
        HEPMC3CODE( Tauolapp::TauolaHepMC3Event t_event(&hepmc); )

        //t_event.undecayTaus();
        t_event.decayTaus();

        return 0;
    }

    void finalize() {
        Tauolapp::Tauola::summary();
        Tauolapp::Log::Summary();
    }
};

#endif
