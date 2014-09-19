// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_SETUP_H
#define  HEPMC_SETUP_H
/**
 *  @file Setup.h
 *  @brief Definition of \b class Setup
 *
 *  @class HepMC::Setup
 *  @brief Configuration for HepMC
 *
 *  Contains macro definitions for printing debug output.
 *  Static class - configuration is shared among all HepMC events
 *  and program threads
 *
 */
#include <iostream>

namespace HepMC {

class Setup {
//
// Constructors
//
private:
    /** @brief Private constructor */
    Setup() {}
    /** @brief Private destructor */
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
public:
    static const unsigned int DEFAULT_DOUBLE_ALMOST_EQUAL_MAXULPS; //!< Default maxUlps for AlmostEqual2sComplement function (double precision)
    static const double       DOUBLE_EPSILON;                      //!< Default threshold for comparing double variables

private:
    static bool m_is_printing_errors;   //!< Flag for printing error messages
    static bool m_is_printing_warnings; //!< Flag for printing warning messages
    static int  m_debug_level;          //!< Level of debug messages printed out
};

/** @brief Macro for printing error messages */
#define ERROR(MESSAGE)       if( Setup::print_errors() )         { std::cerr << "ERROR::"                 << MESSAGE << std::endl; }

/** @brief Macro for printing warning messages */
#define WARNING(MESSAGE)     if( Setup::print_warnings() )       { std::cout << "WARNING::"               << MESSAGE << std::endl; }

// Debug messages and code that will not go to the release version
#ifndef HEPMC_RELEASE_VERSION

/** @brief Macro for printing debug messages with appropriate debug level */
#define DEBUG(LEVEL,MESSAGE) if( Setup::debug_level()>=(LEVEL) ) { std::cout << "DEBUG(" << LEVEL <<")::" << MESSAGE << std::endl; }

/** @brief Macro for storing code useful for debugging */
#define DEBUG_CODE_BLOCK( x ) x

#else
#define DEBUG( x,y )
#define DEBUG_CODE_BLOCK( x )
#endif

} // namespace HepMC

#endif
