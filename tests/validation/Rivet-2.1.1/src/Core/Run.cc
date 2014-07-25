// -*- C++ -*-
#include "Rivet/Run.hh"
#include "Rivet/AnalysisHandler.hh"
#include "HepMC3/IO/IO_GenEvent.h"
#include "Rivet/Math/MathUtils.hh"
#include <limits>

namespace Rivet {


  Run::Run(AnalysisHandler& ah)
    : _ah(ah), _fileweight(1.0), _xs(NAN)
  { }


  Run::~Run() { }


  Run& Run::setCrossSection(const double xs) {
    _xs = xs;
    return *this;
  }


  double Run::crossSection() const {
    return _ah.crossSection();
  }


  Run& Run::setListAnalyses(const bool dolist) {
    _listAnalyses = dolist;
    return *this;
  }


  // Fill event and check for a bad read state
  bool Run::readEvent() {
    /// @todo Clear rather than new the GenEvent object per-event?
    _evt.reset(new GenEvent(HepMC3::Units::GEV,HepMC3::Units::MM));
    if (_io->rdstate() != 0 || !_io->fill_next_event(*_evt.get()) ) {
      Log::getLog("Rivet.Run") << Log::DEBUG << "Read failed. End of file?" << endl;
      return false;
    }
    // Rescale event weights by file-level weight, if scaling is non-trivial
    if (!fuzzyEquals(_fileweight, 1.0)) {
      for (size_t i = 0; i < (size_t) _evt->weights().size(); ++i) {
        _evt->weights()[i] *= _fileweight;
      }
    }
    return true;
  }


  bool Run::openFile(const std::string& evtfile, double weight) {
    // Set current weight-scaling member
    _fileweight = weight;

    // Set up HepMC input reader objects
    if (evtfile == "-") {
      exit(-8);
    } else {
      // Ignore the HepMC3::IO_GenEvent(filename, ios) constructor, since it's only available from HepMC 2.4
      _io.reset(new HepMC3::IO_GenEvent(evtfile.c_str(), std::ios::in));
    }
    if (_io->rdstate() != 0) {
      Log::getLog("Rivet.Run") << Log::ERROR << "Read error on file " << evtfile << endl;
      return false;
    }
    return true;
  }


  bool Run::init(const std::string& evtfile, double weight) {
    if (!openFile(evtfile, weight)) return false;

    // Read first event to define run conditions
    bool ok = readEvent();
    if (!ok) return false;
    if (_evt->particles().size() == 0) {
      Log::getLog("Rivet.Run") << Log::ERROR << "Empty first event." << endl;
      return false;
    }

    // Initialise AnalysisHandler with beam information from first event
    _ah.init(*_evt);

    // Set cross-section from command line
    if (!isnan(_xs)) {
      Log::getLog("Rivet.Run")
        << Log::DEBUG << "Setting user cross-section = " << _xs << " pb" << endl;
      _ah.setCrossSection(_xs);
    }

    // List the chosen & compatible analyses if requested
    if (_listAnalyses) {
      foreach (const std::string& ana, _ah.analysisNames()) {
        cout << ana << endl;
      }
    }

    return true;
  }


  bool Run::processEvent() {
    // Set cross-section if found in event and not from command line
    #ifdef HEPMC_HAS_CROSS_SECTION
    if (std::isnan(_xs) && _evt->cross_section()) {
      const double xs = _evt->cross_section()->cross_section(); //< in pb
      Log::getLog("Rivet.Run")
        << Log::DEBUG << "Setting cross-section = " << xs << " pb" << endl;
      _ah.setCrossSection(xs);
    }
    #endif
    // Complain about absence of cross-section if required!
    if (_ah.needCrossSection() && !_ah.hasCrossSection()) {
      Log::getLog("Rivet.Run")
        << Log::ERROR
        << "Total cross-section needed for at least one of the analyses. "
        << "Please set it (on the command line with '-x' if using the 'rivet' program)" << endl;
      return false;
    }

    // Analyze event
    _ah.analyze(*_evt);

    return true;
  }


  bool Run::finalize() {
    _evt.reset();
    _istr.reset();
    _io.reset();

    if (!isnan(_xs)) _ah.setCrossSection(_xs);
    _ah.finalize();

    return true;
  }




}
