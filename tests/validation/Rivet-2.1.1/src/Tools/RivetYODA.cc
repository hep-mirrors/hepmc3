#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/RivetYODA.hh"
#include "Rivet/Tools/RivetPaths.hh"
#include "YODA/ReaderYODA.h"
#include "YODA/ReaderAIDA.h"
#include "boost/algorithm/string/split.hpp"

using namespace std;

namespace Rivet {


  string getDatafilePath(const string& papername) {
    /// Try to find YODA otherwise fall back to try AIDA
    const string path1 = findAnalysisRefFile(papername + ".yoda");
    if (!path1.empty()) return path1;
    const string path2 = findAnalysisRefFile(papername + ".aida");
    if (!path2.empty()) return path2;
    throw Rivet::Error("Couldn't find ref data file '" + papername + ".yoda/aida" +
                       " in $RIVET_REF_PATH, '" + getRivetDataPath() + "', or '.'");
  }


  std::map<std::string, Scatter2DPtr> getRefData(const string& papername) {
    const string datafile = getDatafilePath(papername);

    // Make an appropriate data file reader and read the data objects
    YODA::Reader& reader = (datafile.find(".yoda") != string::npos) ? \
      YODA::ReaderYODA::create() : YODA::ReaderAIDA::create();
    vector<YODA::AnalysisObject *> aovec;
    reader.read(datafile, aovec);

    // Return value, to be populated
    std::map<std::string, Scatter2DPtr> rtn;
    foreach ( YODA::AnalysisObject* ao, aovec ) {
      Scatter2DPtr refdata( dynamic_cast<Scatter2D *>(ao) );
      if (!refdata) continue;
      string plotpath = refdata->path();

      // Split path at "/" and only return the last field, i.e. the histogram ID
      std::vector<string> pathvec;
      split( pathvec, plotpath, is_any_of("/"), token_compress_on );
      plotpath = pathvec.back();

      rtn[plotpath] = refdata;
    }
    return rtn;
  }


}
