// -*- C++ -*-
#include "Rivet/AnalysisLoader.hh"
#include "Rivet/Tools/RivetPaths.hh"
#include "Rivet/Tools/Utils.hh"
#include "Rivet/Tools/osdir.hh"
#include "Rivet/Analysis.hh"
#include <dlfcn.h>

namespace Rivet {


  namespace {
    inline Log& getLog() {
      return Log::getLog("Rivet.AnalysisLoader");
    }
  }

  // Initialise static ptr collection
  AnalysisLoader::AnalysisBuilderMap AnalysisLoader::_ptrs;


  vector<string> AnalysisLoader::analysisNames() {
    _loadAnalysisPlugins();
    vector<string> names;
    foreach (const AnalysisBuilderMap::value_type& p, _ptrs) names += p.first;
    return names;
  }


  set<string> AnalysisLoader::getAllAnalysisNames() {
    set<string> anaset;
    vector<string> anas = analysisNames();
    foreach (const string &ana, anas) {
      anaset.insert(ana);
    }
    return anaset;
  }


  Analysis* AnalysisLoader::getAnalysis(const string& analysisname) {
    _loadAnalysisPlugins();
    AnalysisBuilderMap::const_iterator ai = _ptrs.find(analysisname);
    if (ai == _ptrs.end()) return 0;
    return ai->second->mkAnalysis();
  }


  vector<Analysis*> AnalysisLoader::getAllAnalyses() {
    _loadAnalysisPlugins();
    vector<Analysis*> analyses;
    foreach (const AnalysisBuilderMap::value_type& p, _ptrs) {
      analyses += p.second->mkAnalysis();
    }
    return analyses;
  }


  void AnalysisLoader::_registerBuilder(const AnalysisBuilderBase* ab) {
    if (!ab) return;
    const string name = ab->name();
    if (_ptrs.find(name) != _ptrs.end()) {
      // Duplicate analyses will be ignored... loudly
      //cerr << "Ignoring duplicate plugin analysis called '" << name << "'" << endl;
      MSG_WARNING("Ignoring duplicate plugin analysis called '" << name << "'");
    } else {
      MSG_TRACE("Registering a plugin analysis called '" << name << "'");
      _ptrs[name] = ab;
    }
  }


  void AnalysisLoader::_loadAnalysisPlugins() {
    // Only run once
    if (!_ptrs.empty()) return;

    // Build the list of directories to search
    const vector<string> dirs = getAnalysisLibPaths();

    // Find plugin module library files
    const string libsuffix = ".so";
    vector<string> pluginfiles;
    foreach (const string& d, dirs) {
      if (d.empty()) continue;
      oslink::directory dir(d);
      while (dir) {
        string filename = dir.next();
        // Require that plugin lib name starts with 'Rivet'
        if (filename.find("Rivet") != 0) continue;
        size_t posn = filename.find(libsuffix);
        if (posn == string::npos || posn != filename.length()-libsuffix.length()) continue;
        /// @todo Make sure this is an abs path
        /// @todo Sys-dependent path separator instead of "/"
        const string path = d + "/" + filename;
        // Ensure no duplicate paths
        if (find(pluginfiles.begin(), pluginfiles.end(), path) == pluginfiles.end()) {
          pluginfiles += path;
        }
      }
    }

    // Load the plugin files
    MSG_TRACE("Candidate analysis plugin libs: " << pluginfiles);
    foreach (const string& pf, pluginfiles) {
      MSG_TRACE("Trying to load plugin analyses from file " << pf);
      void* handle = dlopen(pf.c_str(), RTLD_LAZY);
      if (!handle) {
        MSG_WARNING("Cannot open " << pf << ": " << dlerror());
        continue;
      }
    }
  }


}
