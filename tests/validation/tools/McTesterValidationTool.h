#ifndef MCTESTER_TOOL_H
#define MCTESTER_TOOL_H

#ifdef MCTESTER_HEPMC2
#include "HepMC/GenEvent.h"
#include "HepMCEvent.H"
#else
#include "HepMC3/GenEvent.h"
#include "HepMC3Event.h"
#endif // ifdef MCTESTER_HEPMC2

#include "ValidationTool.h"

#include "Setup.H"
#include "Generate.h"

class McTesterValidationTool : public ValidationTool {
public:
    const std::string name()   { return "MC-TESTER"; }
    bool tool_modifies_event() { return false; }

    void initialize() {
        HEPMC2CODE( ::Setup::stage = 1; )
        HEPMC3CODE( ::Setup::stage = 2; )
        MC_Initialize();
    }

    int process(GenEvent &hepmc) {
        HEPMC2CODE( HepMCEvent  mc_tester(hepmc); )
        HEPMC3CODE( HepMC3Event mc_tester(hepmc); )

        MC_Analyze(&mc_tester,false);

        return 0;
    }

    void finalize() {
        MC_Finalize();
    }
};

#endif
