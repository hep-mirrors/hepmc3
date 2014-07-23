#ifndef RIVET_LOGGING_HH
#define RIVET_LOGGING_HH

#include "Rivet/Config/RivetCommon.hh"

namespace Rivet {


  class Log {
  public:

    /// Log priority levels.
    enum Level {
      TRACE = 0, DEBUG = 10, INFO = 20, WARN = 30, WARNING = 30, ERROR = 40, CRITICAL = 50, ALWAYS = 50
    };

    /// Typedef for a collection of named logs.
    typedef std::map<std::string, Log*> LogMap;

    /// Typedef for a collection of named log levels.
    typedef std::map<std::string, int> LevelMap;

    /// Typedef for a collection of shell color codes, accessed by log level.
    typedef std::map<int, std::string> ColorCodes;

  private:
    /// A static map of existing logs: we don't make more loggers than necessary.
    static LogMap existingLogs;

    /// A static map of default log levels.
    static LevelMap defaultLevels;

    /// A static map of shell color codes for the log levels.
    static ColorCodes colorCodes;

    /// Shell color code for the end of the log levels.
    static std::string endColorCode;

    /// Show timestamp?
    static bool showTimestamp;

    /// Show log level?
    static bool showLogLevel;

    /// Show logger name?
    static bool showLoggerName;

    /// Use shell colour escape codes?
    static bool useShellColors;

  public:
    /// Set the log levels
    static void setLevel(const std::string& name, int level);
    static void setLevels(const LevelMap& logLevels);

    static void setShowTimestamp(bool showTime=true) {
      showTimestamp = showTime;
    }

    static void setShowLevel(bool showLevel=true) {
      showLogLevel = showLevel;
    }

    static void setShowLoggerName(bool showName=true) {
      showLoggerName = showName;
    }

    static void setUseColors(bool useColors=true) {
      useShellColors = useColors;
    }

  protected:

    /// @name Hidden constructors etc.
    //@{

    /// Constructor 1
    Log(const std::string& name);

    /// Constructor 2
    Log(const std::string& name, int level);

    //@}

    static std::string getColorCode(int level);

  public:

    /// Get a logger with the given name. The level will be taken from the
    /// "requestedLevels" static map or will be INFO by default.
    static Log& getLog(const std::string& name);

  public:
    /// Get the priority level of this logger.
    int getLevel() const {
      return _level;
    }

    /// Set the priority level of this logger.
    Log& setLevel(int level) {
      _level = level;
      return *this;
    }

    /// Get a log level enum from a string.
    static Level getLevelFromName(const std::string& level);

    /// Get the std::string representation of a log level.
    static std::string getLevelName(int level);

    /// Get the name of this logger.
    std::string getName() const {
      return _name;
    }

    /// Set the name of this logger.
    Log& setName(const std::string& name) {
      _name = name;
      return *this;
    }

    /// Will this log level produce output on this logger at the moment?
    bool isActive(int level) const {
      return (level >= _level);
    }

    /// @name Explicit log methods
    //@{
    void trace(const std::string& message) { log(TRACE, message); }

    void debug(const std::string& message) { log(DEBUG, message); }

    void info(const std::string& message) { log(INFO, message); }

    void warn(const std::string& message) { log(WARN, message); }

    void error(const std::string& message) { log(ERROR, message); }
    //@}

  private:
    /// This logger's name
    std::string _name;

    /// Threshold level for this logger.
    int _level;

  protected:
    /// Write a message at a particular level.
    void log(int level, const std::string& message);

    /// Turn a message string into the current log format.
    std::string formatMessage(int level, const std::string& message);

  public:

    /// A null output stream, used for piping discarded output to nowhere.
    /// @todo Hide this...
    std::ostream* const _nostream;

    /// The streaming operator can use Log's internals.
    friend std::ostream& operator<<(Log& log, int level);

  };

  /// Streaming output to a logger must have a Log::Level/int as its first argument.
  std::ostream& operator<<(Log& log, int level);

}


// Neat CPU-conserving logging macros. Use by preference!
// NB. Only usable in classes where a getLog() method is provided
#define MSG_LVL(lvl, x) \
  do { \
    if (getLog().isActive(lvl)) { \
      getLog() << lvl << x << endl;   \
    } \
  } while (0)

#define MSG_TRACE(x)   MSG_LVL(Log::TRACE, x)
#define MSG_DEBUG(x)   MSG_LVL(Log::DEBUG, x)
#define MSG_INFO(x)    MSG_LVL(Log::INFO, x)
#define MSG_WARNING(x) MSG_LVL(Log::WARNING, x)
#define MSG_ERROR(x)   MSG_LVL(Log::ERROR, x)


#endif
