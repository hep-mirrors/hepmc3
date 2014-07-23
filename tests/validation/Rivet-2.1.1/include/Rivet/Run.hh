// -*- C++ -*-
#ifndef RIVET_Run_HH
#define RIVET_Run_HH

#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/Tools/RivetHepMC.hh"

namespace Rivet {


  // Forward declaration
  class AnalysisHandler;


  /// @brief Interface to handle a run of events read from a HepMC stream or file.
  class Run {
  public:

    /// @name Standard constructors and destructors. */
    //@{
    /// The standard constructor.
    Run(AnalysisHandler& ah);

    /// The destructor
    ~Run();
    //@}


  public:

    /// @name Set run properties
    //@{

    /// Get the cross-section for this run.
    Run& setCrossSection(const double xs);

    /// Get the current cross-section from the analysis handler in pb.
    double crossSection() const;

    /// Declare whether to list available analyses
    Run& setListAnalyses(const bool dolist);

    //@}


    /// @name File processing stages
    //@{

    /// Set up HepMC file readers (using the appropriate file weight for the first file)
    bool init(const std::string& evtfile, double weight=1.0);

    /// Open a HepMC GenEvent file (using the appropriate file weight for the first file)
    bool openFile(const std::string& evtfile, double weight=1.0);

    /// Read the next HepMC event
    bool readEvent();

    /// Handle next event
    bool processEvent();

    /// Close up HepMC I/O
    bool finalize();

    //@}


  private:

    /// AnalysisHandler object
    AnalysisHandler& _ah;

    /// @name Run variables obtained from events or command line
    //@{

    /// @brief An extra event weight scaling per event file.
    /// Useful for e.g. AlpGen n-parton event file combination.
    double _fileweight;

    /// Cross-section from command line.
    double _xs;

    //@}


    /// Flag to show list of analyses
    bool _listAnalyses;


    /// @name HepMC I/O members
    //@{

    /// Current event
    shared_ptr<GenEvent> _evt;

    /// Output stream for HepMC writer
    shared_ptr<std::istream> _istr;

    /// HepMC I/O writer
    shared_ptr<HepMC3::IO_GenEvent> _io;

    //@}

  };


}

#endif
