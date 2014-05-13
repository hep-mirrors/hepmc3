#include "FileValidationTool.h"

FileValidationTool::FileValidationTool(const std::string &filename, std::ios::openmode mode):m_file_in(NULL),m_file_out(NULL),m_timer("HepMC event writing time") {
    if(mode == std::ios::in) {
        HEPMC2CODE( m_file_in = new IO_GenEvent(filename, mode);       )
        HEPMC3CODE( m_file_in = new IO_HepMC2_adapter(filename, mode); )

        m_timer = Timer("HepMC event parsing time");
    }
    else {
        m_file_out = new IO_GenEvent(filename, mode);
    }

    m_filename = filename;
}

FileValidationTool::~FileValidationTool() {
    if(m_file_in)  delete m_file_in;
    if(m_file_out) delete m_file_out;
}

void FileValidationTool::initialize() {}

int FileValidationTool::process(GenEvent &hepmc) {
    if(m_file_in) {
        HEPMC2CODE( m_file_in->fill_next_event(&hepmc); )
        HEPMC3CODE( m_file_in->fill_next_event( hepmc); )
        if( m_file_in->rdstate() ) return -1;
    }
    else if(m_file_out) {
        HEPMC2CODE( m_file_out->write_event(&hepmc); )
        HEPMC3CODE( m_file_out->write_event( hepmc); )
        if( m_file_out->rdstate() ) return -1;
    }

    return 0;
}

void FileValidationTool::finalize() {
    HEPMC3CODE(
        if(m_file_in)  m_file_in->close();
        if(m_file_out) m_file_out->close();
    )
}

bool FileValidationTool::rdstate() {
    if(m_file_in)  return m_file_in->rdstate();
    if(m_file_out) return m_file_out->rdstate();

    return true;
}
