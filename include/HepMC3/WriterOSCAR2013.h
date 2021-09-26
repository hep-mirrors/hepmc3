// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_WRITEROSCAR2013_H
#define HEPMC3_WRITEROSCAR2013_H
/**
 *  @file  WriterOSCAR2013.h
 *  @brief Definition of \b class WriterOSCAR2013
 *
 *  @class HepMC3::WriterOSCAR2013
 *  @brief GenEvent I/O serialization for OSCAR2013 files
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

class WriterOSCAR2013 : public Writer
{
//
// Constructors
//
public:
    /** @brief Default constructor
     *  @warning If file exists, it will be overwritten
     */
    WriterOSCAR2013(const std::string &filename,
                    std::shared_ptr<GenRunInfo> run = nullptr);

    /// @brief Constructor from ostream
    WriterOSCAR2013(std::ostream& stream,
                    std::shared_ptr<GenRunInfo> run = nullptr);
    /// @brief Constructor from temp ostream
    WriterOSCAR2013(std::shared_ptr<std::ostream> s_stream,
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
    mutable std::map<int,int> m_pdg_to_charge;
    int charge(const int) const;
};

} // namespace HepMC3
#endif
