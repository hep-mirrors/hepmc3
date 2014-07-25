#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/RivetPaths.hh"
#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/Tools/Utils.hh"
#include "binreloc.h"

namespace Rivet {


  inline string _findFile(const string& filename, const vector<string>& paths) {
    foreach (const string& dir, paths) {
      const string path = dir + "/" + filename;
      if (fileexists(path)) return path;
    }
    return "";
  }


  string getLibPath() {
    BrInitError error;
    br_init_lib(&error);
    char* temp = br_find_lib_dir(DEFAULTLIBDIR);
    const string libdir(temp);
    free (temp);
    return libdir;
  }

  string getDataPath() {
    BrInitError error;
    br_init_lib(&error);
    char* temp = br_find_data_dir(DEFAULTDATADIR);
    const string sharedir(temp);
    free (temp);
    return sharedir;
  }

  string getRivetDataPath() {
    return getDataPath() + "/Rivet";
  }



  void setAnalysisLibPaths(const vector<string>& paths) {
    const string pathstr = pathjoin(paths);
    setenv("RIVET_ANALYSIS_PATH", pathstr.c_str(), 1);
  }

  void addAnalysisLibPath(const string& extrapath) {
    vector<string> paths = getAnalysisLibPaths();
    paths.push_back(extrapath);
    setAnalysisLibPaths(paths);
  }

  vector<string> getAnalysisLibPaths() {
    vector<string> dirs;
    char* env = 0;
    env = getenv("RIVET_ANALYSIS_PATH");
    if (env) {
      // Use the Rivet analysis path variable if set...
      dirs += pathsplit(env);
    }
    // ... otherwise fall back to the Rivet library install path unless the path ends in ::
    if (!env || strlen(env) < 2 || string(env).substr(strlen(env)-2) != "::") {
      dirs += getLibPath();
    }
    return dirs;
  }

  string findAnalysisLibFile(const string& filename) {
    return _findFile(filename, getAnalysisLibPaths());
  }


  vector<string> getAnalysisRefPaths() {
    vector<string> dirs;
    char* env = 0;
    env = getenv("RIVET_REF_PATH");
    if (env) {
      // Use the Rivet analysis path variable if set...
      dirs += pathsplit(env);
    }
    // ... then, unless the path ends in :: ...
    if (!env || strlen(env) < 2 || string(env).substr(strlen(env)-2) != "::") {
      // ... fall back to the Rivet data install path...
      dirs += getRivetDataPath();
      // ... and also add any analysis plugin search dirs for convenience
      dirs += getAnalysisLibPaths();
    }
    return dirs;
  }

  string findAnalysisRefFile(const string& filename,
                             const vector<string>& pathprepend, const vector<string>& pathappend) {
    const vector<string> paths = pathprepend + getAnalysisRefPaths() + pathappend;
    return _findFile(filename, paths);
  }


  vector<string> getAnalysisInfoPaths() {
    vector<string> dirs;
    char* env = 0;
    env = getenv("RIVET_INFO_PATH");
    if (env) {
      // Use the Rivet analysis path variable if set...
      dirs += pathsplit(env);
    }
    // ... then, unless the path ends in :: ...
    if (!env || strlen(env) < 2 || string(env).substr(strlen(env)-2) != "::") {
      // ... fall back to the Rivet data install path...
      dirs += getRivetDataPath();
      // ... and also add any analysis plugin search dirs for convenience
      dirs += getAnalysisLibPaths();
    }
    return dirs;
  }

  string findAnalysisInfoFile(const string& filename,
                              const vector<string>& pathprepend, const vector<string>& pathappend) {
    const vector<string> paths = pathprepend + getAnalysisInfoPaths() + pathappend;
    return _findFile(filename, paths);
  }


  vector<string> getAnalysisPlotPaths() {
    vector<string> dirs;
    char* env = 0;
    env = getenv("RIVET_PLOT_PATH");
    if (env) {
      // Use the Rivet analysis path variable if set...
      dirs += pathsplit(env);
    }
    // ... then, unless the path ends in :: ...
    if (!env || strlen(env) < 2 || string(env).substr(strlen(env)-2) != "::") {
      // ... fall back to the Rivet data install path...
      dirs += getRivetDataPath();
      // ... and also add any analysis plugin search dirs for convenience
      dirs += getAnalysisLibPaths();
    }
    return dirs;
  }

  string findAnalysisPlotFile(const string& filename,
                              const vector<string>& pathprepend, const vector<string>& pathappend) {
    const vector<string> paths = pathprepend + getAnalysisPlotPaths() + pathappend;
    return _findFile(filename, paths);
  }


}
