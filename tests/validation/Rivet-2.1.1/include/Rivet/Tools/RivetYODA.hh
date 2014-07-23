#ifndef RIVET_RIVETYODA_HH
#define RIVET_RIVETYODA_HH

/// @author Andy Buckley
/// @date   2009-01-30
/// @author David Grellscheid
/// @date   2011-07-18

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/RivetBoost.hh"

#include "YODA/AnalysisObject.h"
#include "YODA/WriterYODA.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Point2D.h"
#include <map>


namespace Rivet {


  typedef shared_ptr<YODA::AnalysisObject> AnalysisObjectPtr;
  typedef shared_ptr<YODA::Histo1D> Histo1DPtr;
  typedef shared_ptr<YODA::Profile1D> Profile1DPtr;
  typedef shared_ptr<YODA::Scatter2D> Scatter2DPtr;

  using YODA::WriterYODA;
  typedef YODA::Histo1D Histo1D;
  typedef YODA::Profile1D Profile1D;
  typedef YODA::Scatter2D Scatter2D;
  typedef YODA::Point2D Point2D;

  /// Function to get a map of all the refdata in a paper with the
  /// given @a papername.
  map<string, Scatter2DPtr> getRefData(const string& papername);

  /// Get the file system path to the reference file for this paper.
  string getDatafilePath(const string& papername);

  /// Return the integral over the histogram bins
  /// @deprecated Prefer to directly use the histo's integral() method.
  inline double integral(Histo1DPtr histo) {
    return histo->integral();
  }


}

#endif
