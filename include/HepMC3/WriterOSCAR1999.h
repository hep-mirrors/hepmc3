// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_WRITEROSCAR1999_H
#define HEPMC3_WRITEROSCAR1999_H
/**
 *  @file  WriterOSCAR1999.h
 *  @brief Definition of \b class WriterOSCAR1999
 *
 *  @class HepMC3::WriterOSCAR1999
 *  @brief GenEvent I/O serialization for OSCAR1999 files
 *
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/Writer.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Data/GenEventData.h"

#include <fstream>
namespace HepMC3
{

class WriterOSCAR1999 : public Writer
{
//
// Constructors
//
public:
    /** @brief Default constructor
     *  @warning If file exists, it will be overwritten
     */
    WriterOSCAR1999(const std::string &filename,
                    std::shared_ptr<GenRunInfo> run = nullptr);

    /// @brief Constructor from ostream
    WriterOSCAR1999(std::ostream& stream,
                    std::shared_ptr<GenRunInfo> run = nullptr);
    /// @brief Constructor from temp ostream
    WriterOSCAR1999(std::shared_ptr<std::ostream> s_stream,
                    std::shared_ptr<GenRunInfo> run = nullptr);
//
// Functions
//
public:

    /** @brief Write event to file
     *
     *  @param[in] evt Event to be serialized
     */
    void write_event(const GenEvent &evt)  override;

    /** @brief Close file stream */
    void close()  override;

    /** @brief Get stream error state flag */
    bool failed()  override;

protected:
    std::string format_run_info() const;
    std::ofstream m_file; //!< Output file
    std::shared_ptr<std::ostream> m_shared_stream;///< Output temp. stream
    std::ostream* m_stream; //!< Output stream
};

} // namespace HepMC3
#endif
