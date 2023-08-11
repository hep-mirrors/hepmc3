#include <memory>
#include <string>
#include <sys/stat.h>
#include <string.h>

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#include "HepMC3/ReaderPlugin.h"
#include "HepMC3/ReaderFactory_fwd.h"

namespace HepMC3 {
InputInfo::InputInfo (const std::string &filename) {

    if (filename.find("http://") != std::string::npos)    m_remote = true;
    if (filename.find("https://") != std::string::npos)   m_remote = true;
    if (filename.find("root://") != std::string::npos)    m_remote = true;
    if (filename.find("gsidcap://") != std::string::npos) m_remote = true;

    if (!m_remote)
    {
        struct stat   buffer;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && !defined(__CYGWIN__)
        if (!(stat (filename.c_str(), &buffer) == 0))
#else
        if (!(stat (filename.c_str(), &buffer) == 0 && (S_ISFIFO(buffer.st_mode) || S_ISREG(buffer.st_mode) || S_ISLNK(buffer.st_mode))))
#endif
        {
            HEPMC3_ERROR("deduce_reader: file " << filename << " does not exist or is not a regular file/FIFO/link");
            m_reader = std::shared_ptr<Reader> (nullptr);
            m_error = true;
            return;
        }

        std::shared_ptr< std::ifstream > file = std::make_shared< std::ifstream >(filename);
        if (!file)
        {
            HEPMC3_ERROR("deduce_reader could not open file for testing HepMC version: " << filename);
            m_reader = std::shared_ptr<Reader> (nullptr);
            m_error = true;
            return;
        }
        if (!file->is_open()) {
            HEPMC3_ERROR("deduce_reader could not open file for testing HepMC version: " << filename);
            file->close();
            m_reader = std::shared_ptr<Reader> (nullptr);
            m_error = true;
            return;
        }

#if defined(__linux__) || defined(__darwin__)|| defined(__APPLE__) || defined(__FreeBSD__) || defined(__sun)
        m_pipe = S_ISFIFO(buffer.st_mode);
        if (m_pipe) {
            HEPMC3_DEBUG(10, "deduce_reader: the file " << filename << " is a pipe");
            m_reader = deduce_reader(file);
            m_init = true;
            return;
        }
#endif

        std::string line;
        size_t nonempty = 0;
        while (std::getline( *(file.get()), line) && nonempty < 3) {
            if (line.empty()) continue;
            nonempty++;
            m_head.push_back(line);
        }
        file->close();
    }
    // Assure there are at least two elements in the vector:
    m_head.push_back("");
    m_head.push_back("");

    classify();
    m_init = true;
}
void InputInfo::classify() {

    if ( strncmp(m_head.at(0).c_str(), "root", 4) == 0 ) m_root = true;
    if ( strncmp(m_head.at(0).c_str(), "hmpb", 4) == 0 ) m_protobuf = true;
    if ( strncmp(m_head.at(0).c_str(), "HepMC::Version", 14) == 0 && strncmp(m_head.at(1).c_str(), "HepMC::Asciiv3", 14) == 0 ) m_asciiv3 = true;
    if ( strncmp(m_head.at(0).c_str(), "HepMC::Version", 14) == 0 && strncmp(m_head.at(1).c_str(), "HepMC::IO_GenEvent", 18) == 0 ) m_iogenevent = true;
    if ( strncmp(m_head.at(0).c_str(), "<LesHouchesEvents", 17) == 0) m_lhef=true;

    std::stringstream st_e(m_head.at(0).c_str());
    char attr = ' ';
    bool HEPEVT = true;
    int m_i, m_p;
    while (true)
    {
        if (!(st_e >> attr)) {
            HEPEVT = false;
            break;
        }
        if (attr == ' ') continue;
        if (attr != 'E') {
            HEPEVT = false;
            break;
        }
        HEPEVT = static_cast<bool>(st_e >> m_i >> m_p);
        break;
    }
    if (HEPEVT) m_hepevt=true;

}


/** @brief This function will deduce the type of input stream based on its content and will return appropriate Reader*/
std::shared_ptr<Reader> deduce_reader(std::istream &stream)
{
    const size_t raw_header_size = 100;
    std::string raw_header(raw_header_size + 1,'\0');
    auto fstream = dynamic_cast<std::ifstream*>(&stream);
    if (fstream) {
        fstream->read(&(raw_header[0]), raw_header_size);
    } else {
        stream.read(&(raw_header[0]), raw_header_size);
    }
    std::vector<std::string> head;
    head.push_back("");
    for ( size_t i = 0; i < raw_header_size; ++i) {
        const char c = raw_header[i];
        if (c == '\0') break;
        if (c == '\n') {
            if (head.back().length() != 0) {
                head.push_back("");
            }
        } else {
            head.back() += c;
        }
    }
    head.push_back("");
    if (fstream)  {
        for (size_t i = 0; i < raw_header_size; ++i)  { static_cast<std::filebuf*>(fstream->rdbuf())->sungetc(); }
        HEPMC3_DEBUG(10, "After sungetc() fstream->good()=" + std::to_string(fstream->good()));
    } else {
        for (size_t i = 0; i < raw_header_size; ++i)  { stream.rdbuf()->sungetc(); }
        HEPMC3_DEBUG(10, "After sungetc() stream.good()=" + std::to_string(stream.good()));
    }
    if (!stream)
    {
        HEPMC3_WARNING("Input stream is too short or invalid.");
        return std::shared_ptr<Reader>(nullptr);
    }
    InputInfo input;
    input.m_head = head;
    input.classify();
    if (input.m_protobuf) {
        return std::make_shared<ReaderPlugin>(stream,libHepMC3protobufIO,std::string("newReaderprotobufstream"));
    }
    return input.native_reader(stream);
}
/** @brief This function will deduce the type of input stream based on its content and will return appropriate Reader*/
std::shared_ptr<Reader> deduce_reader(std::shared_ptr<std::istream> stream)
{
    if (!stream)
    {
        HEPMC3_WARNING("Input stream is too short or invalid.");
        return std::shared_ptr<Reader>(nullptr);
    }
    const size_t raw_header_size = 100;
    std::string raw_header(raw_header_size + 1,'\0');
    auto fstream = std::dynamic_pointer_cast<std::ifstream>(stream);
    if (fstream) {
        fstream->read(&(raw_header[0]), raw_header_size);
    } else {
        stream->read(&(raw_header[0]), raw_header_size);
    }
    std::vector<std::string> head;
    head.push_back("");
    for ( size_t i = 0; i < raw_header_size; ++i) {
        const char c = raw_header[i];
        if (c == '\0') break;
        if (c == '\n') {
            if (head.back().length() != 0) {
                head.push_back("");
            }
        } else {
            head.back() += c;
        }
    }
    head.push_back("");
    if (fstream)  {
        for (size_t i = 0; i < raw_header_size; ++i)  { static_cast<std::filebuf*>(fstream->rdbuf())->sungetc(); }
        HEPMC3_DEBUG(10, "After sungetc() fstream->good()="+ std::to_string(fstream->good()));
    } else {
        for (size_t i = 0; i < raw_header_size; ++i)  { stream->rdbuf()->sungetc(); }
        HEPMC3_DEBUG(10, "After sungetc() stream->good()="+ std::to_string(stream->good()));
    }

    if (!stream)
    {
        HEPMC3_WARNING("Input stream is too short or invalid.");
        return std::shared_ptr<Reader>(nullptr);
    }

    InputInfo input;
    input.m_head = head;
    input.classify();
    if (input.m_protobuf) {
        return std::make_shared<ReaderPlugin>(stream,libHepMC3protobufIO,std::string("newReaderprotobufspstream"));
    }
    return input.native_reader(stream);
}
}
