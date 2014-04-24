#ifndef FILE_VALIDTAION_TOOL_H
#define FILE_VALIDTAION_TOOL_H

#ifdef HEPMC2
#include "HepMC/IO_GenEvent.h"
#else
#include "HepMC3/IO/IO_GenEvent.h"
#include "HepMC3/IO/IO_HepMC2_adapter.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"

class FileValidationTool : public ValidationTool {
public:
    FileValidationTool(const std::string &filename, std::ios::openmode mode):file_in(NULL),file_out(NULL) {
        if(mode == std::ios::in) {
            HEPMC2CODE( file_in = new IO_GenEvent(filename, mode);       )
            HEPMC3CODE( file_in = new IO_HepMC2_adapter(filename, mode); )
        }
        else {
            file_out = new IO_GenEvent(filename, mode);
        }

        m_filename = filename;
    }

    ~FileValidationTool() {
        if(file_in)  delete file_in;
        if(file_out) delete file_out;
    }

    const std::string name() {
        if(file_in) return "FileValidationTool: input file " + m_filename;
        else        return "FileValidationTool: output file " + m_filename;
    }

    bool tool_modifies_event() { return (file_in) ? true : false; }

    void initialize() {}

    int process(GenEvent &hepmc) {
        if(file_in) {
            HEPMC2CODE( file_in->fill_next_event(&hepmc); )
            HEPMC3CODE( file_in->fill_next_event( hepmc); )
            if( file_in->rdstate() ) return -1;
        }
        else if(file_out) {
            HEPMC2CODE( file_out->write_event(&hepmc); )
            HEPMC3CODE( file_out->write_event( hepmc); )
            if( file_out->rdstate() ) return -1;
        }

        return 0;
    }

    void finalize() {
        HEPMC3CODE(
            if(file_in)  file_in->close();
            if(file_out) file_out->close();
        )
    }

    bool rdstate() {
        if(file_in)  return file_in->rdstate();
        if(file_out) return file_out->rdstate();

        return true;
    }

private:
    HEPMC2CODE( IO_GenEvent       *file_in; )
    HEPMC3CODE( IO_HepMC2_adapter *file_in; )

    IO_GenEvent *file_out;
    std::string m_filename;
};

#endif
