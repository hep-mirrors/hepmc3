// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/// @file Setup.h
/// @brief Definition of \b class Setup

#ifndef HEPMC_SETUP_H
#define HEPMC_SETUP_H

#include <iostream>
#include <stdexcept>

namespace HepMC {


    /// @todo Move some of the helper functions, logging, exceptions, etc. to specific files


    /// @brief Configuration for HepMC
    ///
    /// Contains macro definitions for printing debug output, feature deprecation, etc.
    /// Static class - configuration is shared among all HepMC events
    /// and program threads
    ///
    /// @todo Change to a namespace
    class Setup {

        /// Private constructor
        Setup() {}
        /// Private destructor
        ~Setup() {}


    public:

        /// @name Accessors
        //@{

        /// Get error messages printing flag
        static bool print_errors()                { return m_is_printing_errors;    }
        /// set error messages printing flag
        static void set_print_errors(bool flag)   { m_is_printing_errors   = flag;  }

        /// Get warning messages printing flag
        static bool print_warnings()              { return m_is_printing_warnings;  }
        /// Set warning messages printing flag
        static void set_print_warnings(bool flag) { m_is_printing_warnings = flag;  }

        /// Get debug level
        static int  debug_level()                 { return m_debug_level;           }
        /// Set debug level
        static void set_debug_level(int level)    { m_debug_level          = level; }
        //@}

        /// @name Static constants
        //@{
        /// Default maxUlps for AlmostEqual2sComplement function (double precision)
        static const unsigned int DEFAULT_DOUBLE_ALMOST_EQUAL_MAXULPS;

        /// Default threshold for comparing double variables
        static const double DOUBLE_EPSILON;

        //@}


    private:

        static bool m_is_printing_errors;   //!< Flag for printing error messages
        static bool m_is_printing_warnings; //!< Flag for printing warning messages
        static int  m_debug_level;          //!< Level of debug messages printed out
    };




    /// @name Printing macros
    //@{

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

    //@}


    /// @name Exceptions
    /// @todo Move to a dedicated header once we have more than one
    //@{

    /// @todo Should it be GenException? It's a HepMC thing, but not really a "gen" thing
    struct Exception : public std::runtime_error {
      Exception(const std::string& msg) : std::runtime_error(msg) {}
    };

    //@}


    /// Deprecation macro
    #ifndef HEPMC_DEPRECATED
    #if __GNUC__ && __cplusplus && HEPMC_NO_DEPRECATION_WARNINGS == 0
    #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
    #if GCC_VERSION >= 40500
      #define HEPMC_DEPRECATED(x) __attribute__((deprecated(x)))
    #else
      #define HEPMC_DEPRECATED(x) __attribute__((deprecated))
    #endif
    #else
      #define HEPMC_DEPRECATED(x)
    #endif
    #endif


} // namespace HepMC

#endif
