// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/AnalysisHandler.hh"
#include "Rivet/Analysis.hh"
#include "Rivet/ParticleName.hh"
#include "Rivet/BeamConstraint.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/Beam.hh"

namespace Rivet {


  namespace {
    inline bool cmpAOByPath(const AnalysisObjectPtr a, const AnalysisObjectPtr b) {
      return a->path() < b->path();
    }
  }


  AnalysisHandler::AnalysisHandler(const string& runname)
    : _runname(runname), _numEvents(0),
      _sumOfWeights(0.0), _xs(NAN),
      _initialised(false), _ignoreBeams(false)
  {}

  AnalysisHandler::~AnalysisHandler()
  {}


  Log& AnalysisHandler::getLog() const {
    return Log::getLog("Rivet.Analysis.Handler");
  }


  void AnalysisHandler::init(const GenEvent& ge) {
    if (_initialised)
      throw UserError("AnalysisHandler::init has already been called: cannot re-initialize!");

    setRunBeams(Rivet::beams(ge));
    MSG_DEBUG("Initialising the analysis handler");
    _numEvents = 0;
    _sumOfWeights = 0.0;

    // Check that analyses are beam-compatible, and remove those that aren't
    const size_t num_anas_requested = analysisNames().size();
    vector<string> anamestodelete;
    foreach (const AnaHandle a, _analyses) {
      if (!_ignoreBeams && !a->isCompatible(beams())) {
        //MSG_DEBUG(a->name() << " requires beams " << a->requiredBeams() << " @ " << a->requiredEnergies() << " GeV");
        anamestodelete.push_back(a->name());
      }
    }
    foreach (const string& aname, anamestodelete) {
      MSG_WARNING("Analysis '" << aname << "' is incompatible with the provided beams: removing");
      removeAnalysis(aname);
    }
    if (num_anas_requested > 0 && analysisNames().empty()) {
      cerr << "All analyses were incompatible with the first event's beams\n"
           << "Exiting, since this probably wasn't intentional!" << endl;
      exit(1);
    }

    // Warn if any analysis' status is not unblemished
    foreach (const AnaHandle a, analyses()) {
      if (toUpper(a->status()) == "PRELIMINARY") {
        MSG_WARNING("Analysis '" << a->name() << "' is preliminary: be careful, it may change and/or be renamed!");
      } else if (toUpper(a->status()) == "OBSOLETE") {
        MSG_WARNING("Analysis '" << a->name() << "' is obsolete: please update!");
      } else if (toUpper(a->status()).find("UNVALIDATED") != string::npos) {
        MSG_WARNING("Analysis '" << a->name() << "' is unvalidated: be careful, it may be broken!");
      }
    }

    // Initialize the remaining analyses
    foreach (AnaHandle a, _analyses) {
      MSG_DEBUG("Initialising analysis: " << a->name());
      try {
        // Allow projection registration in the init phase onwards
        a->_allowProjReg = true;
        a->init();
        //MSG_DEBUG("Checking consistency of analysis: " << a->name());
        //a->checkConsistency();
      } catch (const Error& err) {
        cerr << "Error in " << a->name() << "::init method: " << err.what() << endl;
        exit(1);
      }
      MSG_DEBUG("Done initialising analysis: " << a->name());
    }
    _initialised = true;
    MSG_DEBUG("Analysis handler initialised");
  }


  void AnalysisHandler::analyze(const GenEvent& ge) {
    // Call init with event as template if not already initialised
    if (!_initialised) {
      init(ge);
    }
    // Proceed with event analysis
    assert(_initialised);
    // Ensure that beam details match those from first event
    const PdgIdPair beams = Rivet::beamIds(ge);
    const double sqrts = Rivet::sqrtS(ge);
    if (!compatible(beams, _beams) || !fuzzyEquals(sqrts, sqrtS())) {
      cerr << "Event beams mismatch: "
           << PID::toBeamsString(beams) << " @ " << sqrts/GeV << " GeV" << " vs. first beams "
           << this->beams() << " @ " << this->sqrtS()/GeV << " GeV" << endl;
      exit(1);
    }


    Event event(ge);
    _numEvents++;
    // Weights
    const double weight = event.weight();
    _sumOfWeights += weight;
    MSG_DEBUG("Event #" << _numEvents << " weight = " << weight);
    #ifdef HEPMC_HAS_CROSS_SECTION
    if (ge.cross_section()) {
      _xs = ge.cross_section()->cross_section();
    }
    #endif
    foreach (AnaHandle a, _analyses) {
      //MSG_DEBUG("About to run analysis " << a->name());
      try {
        a->analyze(event);
      } catch (const Error& err) {
        cerr     << "Error in " << a->name() << "::analyze method: "
                 << err.what() << endl;
        exit(1);
      }
      //MSG_DEBUG("Finished running analysis " << a->name());
    }
  }


  void AnalysisHandler::finalize() {
    if (!_initialised) return;
    MSG_INFO("Finalising analyses");
    foreach (AnaHandle a, _analyses) {
      a->setCrossSection(_xs);
      try {
        a->finalize();
      } catch (const Error& err) {
        cerr << "Error in " << a->name() << "::finalize method: "
             << err.what() << endl;
        exit(1);
      }
    }

    // Print out number of events processed
    MSG_INFO("Processed " << _numEvents << " event" << (_numEvents == 1 ? "" : "s"));

    // // Delete analyses
    // MSG_DEBUG("Deleting analyses");
    // _analyses.clear();

    // Print out MCnet boilerplate
    cout << endl;
    cout << "The MCnet usage guidelines apply to Rivet: see http://www.montecarlonet.org/GUIDELINES" << endl;
    cout << "Please acknowledge plots made with Rivet analyses, and cite arXiv:1003.0694 (http://arxiv.org/abs/1003.0694)" << endl;
  }


  AnalysisHandler& AnalysisHandler::addAnalysis(const string& analysisname) {
    // Check for a duplicate analysis
    /// @todo Might we want to be able to run an analysis twice, with different params?
    ///       Requires avoiding histo tree clashes, i.e. storing the histos on the analysis objects.
    foreach (const AnaHandle& a, _analyses) {
      if (a->name() == analysisname) {
        MSG_WARNING("Analysis '" << analysisname << "' already registered: skipping duplicate");
        return *this;
      }
    }
    AnaHandle analysis( AnalysisLoader::getAnalysis(analysisname) );
    if (analysis.get() != 0) { // < Check for null analysis.
      MSG_DEBUG("Adding analysis '" << analysisname << "'");
      analysis->_analysishandler = this;
      _analyses.insert(analysis);
    } else {
      MSG_WARNING("Analysis '" << analysisname << "' not found.");
    }
    // MSG_WARNING(_analyses.size());
    // foreach (const AnaHandle& a, _analyses) MSG_WARNING(a->name());
    return *this;
  }


  AnalysisHandler& AnalysisHandler::removeAnalysis(const string& analysisname) {
    shared_ptr<Analysis> toremove;
    foreach (const AnaHandle a, _analyses) {
      if (a->name() == analysisname) {
        toremove = a;
        break;
      }
    }
    if (toremove.get() != 0) {
      MSG_DEBUG("Removing analysis '" << analysisname << "'");
      _analyses.erase(toremove);
    }
    return *this;
  }


  vector<AnalysisObjectPtr> AnalysisHandler::getData() const {
    vector<AnalysisObjectPtr> rtn;
    foreach (const AnaHandle a, analyses()) {
      vector<AnalysisObjectPtr> aos = a->analysisObjects();
      // MSG_WARNING(a->name() << " " << aos.size());
      foreach (const AnalysisObjectPtr ao, aos) {
        // Exclude paths starting with /TMP/ from final write-out
        /// @todo This needs to be much more nuanced for re-entrant histogramming
        if (ao->path().find("/TMP/") != string::npos) continue;
        rtn.push_back(ao);
      }
    }
    sort(rtn.begin(), rtn.end(), cmpAOByPath);
    return rtn;
  }


  void AnalysisHandler::writeData(const string& filename) const {
    const vector<AnalysisObjectPtr> aos = getData();
    WriterYODA::write(filename, aos.begin(), aos.end());
  }


  string AnalysisHandler::runName() const { return _runname; }
  size_t AnalysisHandler::numEvents() const { return _numEvents; }
  double AnalysisHandler::sumOfWeights() const { return _sumOfWeights; }


  void AnalysisHandler::setSumOfWeights(const double& sum) {
    _sumOfWeights=sum;
  }


  std::vector<std::string> AnalysisHandler::analysisNames() const {
    std::vector<std::string> rtn;
    foreach (AnaHandle a, _analyses) {
      rtn.push_back(a->name());
    }
    return rtn;
  }


  AnalysisHandler& AnalysisHandler::addAnalyses(const std::vector<std::string>& analysisnames) {
    foreach (const string& aname, analysisnames) {
      //MSG_DEBUG("Adding analysis '" << aname << "'");
      addAnalysis(aname);
    }
    return *this;
  }


  AnalysisHandler& AnalysisHandler::removeAnalyses(const std::vector<std::string>& analysisnames) {
    foreach (const string& aname, analysisnames) {
      removeAnalysis(aname);
    }
    return *this;
  }


  bool AnalysisHandler::needCrossSection() const {
    bool rtn = false;
    foreach (const AnaHandle a, _analyses) {
      if (!rtn) rtn = a->needsCrossSection();
      if (rtn) break;
    }
    return rtn;
  }


  AnalysisHandler& AnalysisHandler::setCrossSection(double xs) {
    _xs = xs;
    return *this;
  }


  bool AnalysisHandler::hasCrossSection() const {
    return (!std::isnan(crossSection()));
  }


  AnalysisHandler& AnalysisHandler::addAnalysis(Analysis* analysis) {
    analysis->_analysishandler = this;
    _analyses.insert(AnaHandle(analysis));
    return *this;
  }


  PdgIdPair AnalysisHandler::beamIds() const {
    return Rivet::beamIds(beams());
  }


  double AnalysisHandler::sqrtS() const {
    return Rivet::sqrtS(beams());
  }

  void AnalysisHandler::setIgnoreBeams(bool ignore) {
    _ignoreBeams=ignore;
  }


}
