#ifndef  HEPMC3_SETUP_H
#define  HEPMC3_SETUP_H
/**
 *  @file Setup.h
 *  @brief Definition of \b class HepMC3::Setup
 *
 *  @class HepMC3::Setup
 *  @brief Configuration for HepMC3
 *
 *  Contains macro definitions for printing debug output.
 *  Static class - configuration is shared among all HepMC3 events
 *  and program threads
 *
 */
#include <iostream>
namespace HepMC3 {

class Setup {
//
// Constructors
//
private:
    /** Private constructor */
    Setup() {}
    /** Private destructor */
    ~Setup() {}

//
// Accessors
//
public:
    static bool print_errors()                { return m_is_printing_errors;    } //!< Get error messages printing flag
    static void set_print_errors(bool flag)   { m_is_printing_errors   = flag;  } //!< Set error messages printing flag

    static bool print_warnings()              { return m_is_printing_warnings;  } //!< Get warning messages printing flag
    static void set_print_warnings(bool flag) { m_is_printing_warnings = flag;  } //!< Get warning messages printing flag

    static int  debug_level()                 { return m_debug_level;           } //!< Get debug level
    static void set_debug_level(int level)    { m_debug_level          = level; } //!< Set debug level

//
// Fields
//
private:
    static bool m_is_printing_errors;   //!< Flag for printing error messages
    static bool m_is_printing_warnings; //!< Flag for printing warning messages
    static int  m_debug_level;          //!< Level of debug messages printed out
};

/** Macro for printing error messages */
#define ERROR(MESSAGE)       if( HepMC3::Setup::print_errors() )         { std::cerr << "ERROR::HepMC3::"                 << MESSAGE << std::endl; }

/** Macro for printing warning messages */
#define WARNING(MESSAGE)     if( HepMC3::Setup::print_warnings() )       { std::cout << "WARNING::HepMC3::"               << MESSAGE << std::endl; }

// Debug messages and code that will not go to the release version
#ifndef HEPMC3_RELEASE_VERSION

/** Macro for printing debug messages with appropriate debug level */
#define DEBUG(LEVEL,MESSAGE) if( HepMC3::Setup::debug_level()>=(LEVEL) ) { std::cout << "DEBUG(" << LEVEL <<")::HepMC3::" << MESSAGE << std::endl; }

/** Macro for storing code useful for debugging */
#define DEBUG_CODE_BLOCK( x ) x

#else
#define DEBUG( x )
#define DEBUG_CODE_BLOCK( x )
#endif

} // namespace HepMC3

#endif
