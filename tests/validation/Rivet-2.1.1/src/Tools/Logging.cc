#include "Rivet/Tools/Logging.hh"
#include <ctime>
#include <unistd.h>
using namespace std;

namespace Rivet {


  Log::LogMap Log::existingLogs;
  Log::LevelMap Log::defaultLevels;
  Log::ColorCodes Log::colorCodes;
  string Log::endColorCode;
  bool Log::showTimestamp = false;
  bool Log::showLogLevel = true;
  bool Log::showLoggerName = true;
  bool Log::useShellColors = true;


  Log::Log(const string& name)
    : _name(name), _level(INFO), _nostream(new ostream(0)) { }


  Log::Log(const string& name, int level)
    : _name(name), _level(level), _nostream(new ostream(0)) { }


  /// @todo Add single static setLevel
  void _updateLevels(const Log::LevelMap& defaultLevels, Log::LogMap& existingLogs) {
    /// @todo Check ordering - "Foo" should come before "Foo.Bar"
    for (Log::LevelMap::const_iterator lev = defaultLevels.begin(); lev != defaultLevels.end(); ++lev) {
      for (Log::LogMap::iterator log = existingLogs.begin(); log != existingLogs.end(); ++log) {
        if (log->first.find(lev->first) == 0) {
          log->second->setLevel(lev->second);
        }
      }
    }
  }


  void Log::setLevel(const string& name, int level) {
    defaultLevels[name] = level;
    //cout << name << " -> " << level << endl;
    _updateLevels(defaultLevels, existingLogs);
  }


  void Log::setLevels(const LevelMap& logLevels) {
    for (LevelMap::const_iterator lev = logLevels.begin(); lev != logLevels.end(); ++lev) {
      defaultLevels[lev->first] = lev->second;
    }
    _updateLevels(defaultLevels, existingLogs);
  }


  Log& Log::getLog(const string& name) {
    if (existingLogs.find(name) == existingLogs.end()) {
      int level = INFO;
      // Try running through all parent classes to find an existing level
      string tmpname = name;
      bool triedAllParents = false;
      while (! triedAllParents) {
        // Is there a default level?
        if (defaultLevels.find(tmpname) != defaultLevels.end()) {
          level = defaultLevels.find(tmpname)->second;
          break;
        }
        // Is there already such a logger? (NB. tmpname != name)
        if (existingLogs.find(tmpname) != existingLogs.end()) {
          level = existingLogs.find(tmpname)->second->getLevel();
          break;
        }
        // Crop the string back to the next parent level
        size_t lastDot = tmpname.find_last_of(".");
        if (lastDot != string::npos) {
          tmpname = tmpname.substr(0, lastDot);
        } else {
          triedAllParents = true;
        }
      }
      // for (LevelMap::const_iterator l = defaultLevels.begin(); l != defaultLevels.end(); ++l) {
      // 
      // }
      existingLogs[name] = new Log(name, level);
    }
    return *existingLogs[name];
  }


  string Log::getLevelName(int level) {
    /// @todo Do the map::upper_limit thing to find nearest level...
    switch(level) {
    case TRACE:
      return "TRACE";
    case DEBUG:
      return "DEBUG";
    case INFO:
      return "INFO";
    case WARN:
      return "WARN";
    case ERROR:
      return "ERROR";
    default:
      return "";
    }
    //throw Error("Enum value was not a valid log level. How did that happen?");
  }


  string Log::getColorCode(int level) {
    if (!Log::useShellColors) return "";
    // If the codes haven't been initialized, do so now.
    if (Log::colorCodes.empty()) {
      // If stdout is a valid tty, try to use the appropriate codes.
      if (isatty(1)) {
        /// @todo Test for VT100 compliance?
        Log::colorCodes[TRACE] = "\033[0;36m";
        Log::colorCodes[DEBUG] = "\033[0;34m";
        Log::colorCodes[INFO]  = "\033[0;32m";
        Log::colorCodes[WARN]  = "\033[0;33m";
        Log::colorCodes[ERROR] = "\033[0;31m";
        Log::endColorCode      = "\033[0m";
      } else {
        Log::colorCodes[TRACE] = "";
        Log::colorCodes[DEBUG] = "";
        Log::colorCodes[INFO] = "";
        Log::colorCodes[WARN] = "";
        Log::colorCodes[ERROR] = "";
      }
    }
    // Return the appropriate code from the colour map.
    /// @todo Do the map::upper_limit thing to find nearest level...
    return colorCodes[level];
  }


  Log::Level Log::getLevelFromName(const string& level) {
    if (level == "TRACE") return TRACE;
    if (level == "DEBUG") return DEBUG;
    if (level == "INFO") return INFO;
    if (level == "WARN") return WARN;
    if (level == "ERROR") return ERROR;
    throw Error("Couldn't create a log level from string '" + level + "'");
  }


  string Log::formatMessage(int level, const string& message) {
    string out;
    if (Log::useShellColors) {
      out += getColorCode(level);
    }

    if (Log::showLoggerName) {
      out += getName();
      out += ": ";
    }

    if (Log::showLogLevel) {
      out += Log::getLevelName(level);
      out += " ";
    }

    if (Log::showTimestamp) {
      time_t rawtime;
      time(&rawtime);
      char* timestr = ctime(&rawtime);
      timestr[24] = ' ';
      out += timestr;
      out += " ";
    }

    if (Log::useShellColors) {
      out += endColorCode;
    }

    out += " ";
    out += message;
 
    return out;
  }


  void Log::log(int level, const string& message) {
    if (isActive(level)) {
      cout << formatMessage(level, message) << endl;
    }
  }


  ostream& operator<<(Log& log, int level) {
    if (log.isActive(level)) {
      cout << log.formatMessage(level, "");
      return cout;
    } else {
      return *(log._nostream);
    }
  }

}
