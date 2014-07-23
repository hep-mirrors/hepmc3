// -*- C++ -*-
#ifndef RIVET_AnalysisLoader_HH
#define RIVET_AnalysisLoader_HH

#include "Rivet/Config/RivetCommon.hh"
#include <map>
#include <string>

namespace Rivet {


  // Forward declarations
  class Analysis;
  class AnalysisBuilderBase;
  class Log;


  /// Internal class which loads and registers analyses from plugin libs
  class AnalysisLoader {
  public:

    /// Get all the available analyses' names.
    static vector<string> analysisNames();
    static set<string> getAllAnalysisNames();

    /// Get an analysis by name.
    /// Warning: a name arg which matches no known analysis will return a null
    /// pointer. Check your return values before using them!
    static Analysis* getAnalysis(const string& analysisname);

    /// Get all the available analyses.
    static vector<Analysis*> getAllAnalyses();


  private:

    /// Allow the analysis builders to call the private _registerBuilder function
    friend class AnalysisBuilderBase;

    /// Register a new analysis builder
    static void _registerBuilder(const AnalysisBuilderBase* a);

    /// Load the available analyses at runtime.
    static void _loadAnalysisPlugins();

    typedef map<string, const AnalysisBuilderBase*> AnalysisBuilderMap;
    static AnalysisBuilderMap _ptrs;

  };


}

#endif
