#ifndef FILE_VALIDTAION_TOOL_H
#define FILE_VALIDTAION_TOOL_H

#ifdef HEPMC2
#include "HepMC/IO_GenEvent.h"
#else
#include "HepMC3/IO/IO_GenEvent.h"
#include "HepMC3/IO/IO_HepMC2_adapter.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"
#include "Timer.h"

class FileValidationTool : public ValidationTool {
public:
    FileValidationTool(const std::string &filename, std::ios::openmode mode);

    ~FileValidationTool();

public:
    const std::string name() { return "FileValidationTool"; }

    const std::string long_name() {
        if(m_file_in) return name() + " input file:  " + m_filename;
        else          return name() + " output file: " + m_filename;
    }

    bool   tool_modifies_event() { return (m_file_in) ? true : false; }
    Timer* timer()               { return &m_timer; }

    void initialize();
    int  process(GenEvent &hepmc);
    void finalize();

    bool rdstate();

private:
    HEPMC2CODE( IO_GenEvent       *m_file_in; )
    HEPMC3CODE( IO_HepMC2_adapter *m_file_in; )

    IO_GenEvent *m_file_out;
    std::string  m_filename;
    Timer        m_timer;
};

#endif
