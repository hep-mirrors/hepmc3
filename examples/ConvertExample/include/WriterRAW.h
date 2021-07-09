#ifndef HEPMC3_WRITERRAW_H
#define HEPMC3_WRITERRAW_H
///
/// @file  WriterRAW.h
/// @brief Definition of class \b WriterRAW
///
/// @class HepMC3::WriterRAW
/// @brief GenEvent I/O output to RAW files that should be processed by graphviz or other software
///
/// @ingroup Examples
///
#include <string>
#include <fstream>
#include "HepMC3/Writer.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenRunInfoData.h"
namespace HepMC3
{
class WriterRAW : public Writer
{
public:
    /// @brief Constructor
    /// @warning If file already exists, it will be cleared before writing
    WriterRAW(const std::string &filename,std::shared_ptr<GenRunInfo> run = std::shared_ptr<GenRunInfo>());
    /// @brief Constructor from ostream
    WriterRAW(std::ostream& stream,std::shared_ptr<GenRunInfo> run =std:: shared_ptr<GenRunInfo>());
    /// @brief Write event to file
    ///
    /// @param[in] evt Event to be serialized
    void write_event(const GenEvent &evt);
    /// @brief Return status of the stream
    bool failed() {
        return (bool)m_file.rdstate();
    }
    /// @brief Close file stream
    void close();
    /// @brief Close file stream

void write_run_info();

private:
    void allocate_buffer(); //!< allocates buffer for output
    void flush(); //!< flushes output buffer
    void forced_flush(); //!< flushes output buffer
    std::ofstream m_file; //!< Output file
    std::ostream* m_stream; //!< Output stream

    char* m_buffer;  //!< Stream buffer
    char* m_cursor;  //!< Cursor inside stream buffer
    unsigned long m_buffer_size; //!< Buffer size
};
}
#endif
