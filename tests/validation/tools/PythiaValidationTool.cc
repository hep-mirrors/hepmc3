#include "PythiaValidationTool.h"

PythiaValidationTool::PythiaValidationTool( const string &filename ):m_filename(filename),m_timer("pythia8 conversion time") {
    m_pythia.readFile(m_filename);
}

void PythiaValidationTool::initialize() {
    m_pythia.init();
}

int PythiaValidationTool::process(GenEvent &hepmc) {
    if( !m_pythia.next() ) return 1;

    // Exclude generation time
    m_timer.start();

    m_tohepmc.fill_next_event(m_pythia.event, &hepmc);

    return 0;
}

void PythiaValidationTool::finalize() {
    m_pythia.statistics();
}
