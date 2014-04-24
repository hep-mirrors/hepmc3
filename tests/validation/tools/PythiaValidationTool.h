#ifndef PYTHIA_VALIDATION_TOOL_H
#define PYTHIA_VALIDATION_TOOL_H

#ifdef PYTHIA8_HEPMC2
#include "HepMC/GenEvent.h"
#include "Pythia8/Pythia8ToHepMC.h"
#else
#include "HepMC3/GenEvent.h"
#include "Pythia8/Pythia8ToHepMC3.h"
#endif // ifdef PYTHIA8_HEPMC2

#include "ValidationTool.h"
#include "Pythia8/Pythia.h"

class PythiaValidationTool : public ValidationTool {
public:
    PythiaValidationTool( const string &filename ) {
        pythia.readFile(filename);
    }

    const std::string name()   { return "pythia8"; }
    bool tool_modifies_event() { return true; }

    void initialize() {
        pythia.init();
    }

    int process(GenEvent &hepmc) {
        if( !pythia.next() ) return 1;

        ToHepMC.fill_next_event( pythia.event, &hepmc);

        return 0;
    }

    void finalize() {
        pythia.statistics();
    }
private:
    Pythia8::Pythia pythia;
    HEPMC2CODE( HepMC::Pythia8ToHepMC   ToHepMC; )
    HEPMC3CODE( HepMC3::Pythia8ToHepMC3 ToHepMC; )
};

#endif
