// -*- C++ -*-
#ifndef RIVET_RivetHandler_HH
#define RIVET_RivetHandler_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/AnalysisLoader.hh"
#include "Rivet/Tools/RivetYODA.hh"

namespace Rivet {


  // Forward declaration and smart pointer for Analysis
  class Analysis;
  typedef shared_ptr<Analysis> AnaHandle;


  // Needed to make smart pointers compare equivalent in the STL set
  struct CmpAnaHandle {
    bool operator() (const AnaHandle& a, const AnaHandle& b) {
      return a.get() < b.get();
    }
  };


  /// A class which handles a number of analysis objects to be applied to
  /// generated events. An {@link Analysis}' AnalysisHandler is also responsible
  /// for handling the final writing-out of histograms.
  class AnalysisHandler {
  public:

    /// @name Constructors and destructors. */
    //@{

    /// Preferred constructor, with optional run name.
    AnalysisHandler(const string& runname="");

    /// @brief Destructor
    /// The destructor is not virtual, as this class should not be inherited from.
    ~AnalysisHandler();

    //@}


  private:

    /// Get a logger object.
    Log& getLog() const;


  public:

    /// @name Run properties
    //@{

    /// Get the name of this run.
    string runName() const;


    /// Get the number of events seen. Should only really be used by external
    /// steering code or analyses in the finalize phase.
    size_t numEvents() const;

    /// Get the sum of the event weights seen - the weighted equivalent of the
    /// number of events. Should only really be used by external steering code
    /// or analyses in the finalize phase.
    double sumOfWeights() const;

    /// Set sum of weights. This is useful if Rivet is steered externally and
    /// the analyses are run for a sub-contribution of the events
    /// (but of course have to be normalised to the total sum of weights)
    void setSumOfWeights(const double& sum);


    /// Is cross-section information required by at least one child analysis?
    bool needCrossSection() const;

    /// Set the cross-section for the process being generated.
    AnalysisHandler& setCrossSection(double xs);

    /// Get the cross-section known to the handler.
    double crossSection() const {
      return _xs;
    }

    /// Whether the handler knows about a cross-section.
    bool hasCrossSection() const;


    /// Set beams for this run
    AnalysisHandler& setRunBeams(const ParticlePair& beams) {
      _beams = beams;
      MSG_DEBUG("Setting run beams = " << beams << " @ " << sqrtS()/GeV << " GeV");
      return *this;
    }

    /// Get beam IDs for this run, usually determined from the first event.
    const ParticlePair& beams() const {
      return _beams;
    }

    /// Get beam IDs for this run, usually determined from the first event.
    PdgIdPair beamIds() const;

    /// Get energy for this run, usually determined from the first event.
    double sqrtS() const;

    /// Setter for _ignoreBeams
    void setIgnoreBeams(bool ignore=true);

    //@}


    /// @name Handle analyses
    //@{

    /// Get a list of the currently registered analyses' names.
    std::vector<std::string> analysisNames() const;

    /// Get the collection of currently registered analyses.
    const std::set<AnaHandle, CmpAnaHandle>& analyses() const {
      return _analyses;
    }

    /// Add an analysis to the run list using its name. The actual Analysis
    /// to be used will be obtained via AnalysisHandler::getAnalysis(string).
    /// If no matching analysis is found, no analysis is added (i.e. the
    /// null pointer is checked and discarded.
    AnalysisHandler& addAnalysis(const std::string& analysisname);

    /// Remove an analysis from the run list using its name.
    AnalysisHandler& removeAnalysis(const std::string& analysisname);


    /// Add analyses to the run list using their names. The actual {@link
    /// Analysis}' to be used will be obtained via
    /// AnalysisHandler::addAnalysis(string), which in turn uses
    /// AnalysisHandler::getAnalysis(string). If no matching analysis is found
    /// for a given name, no analysis is added, but also no error is thrown.
    AnalysisHandler& addAnalyses(const std::vector<std::string>& analysisnames);

    /// Remove analyses from the run list using their names.
    AnalysisHandler& removeAnalyses(const std::vector<std::string>& analysisnames);

    /// Add an analysis to the run list by object
    AnalysisHandler& addAnalysis(Analysis* analysis);

    //@}


    /// @name Main init/execute/finalise
    //@{

    /// Initialize a run, with the run beams taken from the example event.
    void init(const GenEvent& event);

    /// Analyze the given \a event. This function will call the
    /// AnalysisBase::analyze() function of all included analysis objects.
    void analyze(const GenEvent& event);

    /// Finalize a run. This function calls the AnalysisBase::finalize()
    /// functions of all included analysis objects.
    void finalize();

    //@}


    /// @name Histogram / data object access
    //@{

    /// Get all analyses' plots as a vector of analysis objects.
    std::vector<AnalysisObjectPtr> getData() const;

    /// Write all analyses' plots to the named file.
    void writeData(const std::string& filename) const;

    //@}


  private:

    /// The collection of Analysis objects to be used.
    set<AnaHandle, CmpAnaHandle> _analyses;


    /// @name Run properties
    //@{

    /// Run name
    std::string _runname;

    /// Number of events seen.
    unsigned int _numEvents;

    /// Sum of event weights seen.
    double _sumOfWeights;

    /// Cross-section known to AH
    double _xs;

    /// Beams used by this run.
    ParticlePair _beams;

    /// Flag to check if init has been called
    bool _initialised;

    /// Flag whether input event beams should be ignored in compatibility check
    bool _ignoreBeams;

    //@}

  private:

    /// The assignment operator is private and must never be called.
    /// In fact, it should not even be implemented.
    AnalysisHandler& operator=(const AnalysisHandler&);

    /// The copy constructor is private and must never be called.  In
    /// fact, it should not even be implemented.
    AnalysisHandler(const AnalysisHandler&);

  };


}

#endif
