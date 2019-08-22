// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
/**
 *  @file Errors.h
 *  @brief Implementation of  error and warning macros
 *
 */
#ifndef HEPMC3_ERRORS_H
#define HEPMC3_ERRORS_H

#include <iostream>
#include <stdexcept>

namespace HepMC3 {


/// @name Printing macros
//@{

/** @brief Macro for printing error messages */
#define ERROR(MESSAGE)   if ( Setup::print_errors() )   { std::cerr << "ERROR::" << MESSAGE << std::endl; }

/** @brief Macro for printing warning messages */
#define WARNING(MESSAGE) if ( Setup::print_warnings() ) { std::cout << "WARNING::" << MESSAGE << std::endl; }

// Debug messages and code that will not go to the release version
#ifndef HEPMC3_RELEASE_VERSION

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
//@{

/// @class Exception
/// @brief Standard runtime error
struct Exception : public std::runtime_error {
    Exception(const std::string& msg) : std::runtime_error(msg) {} ///< Default constructor
};

/// @brief Exception related to weight lookups, setting, and index consistency
struct WeightError : public Exception {
    WeightError(const std::string& msg) : Exception(msg) {} ///< Default constructor
};

//@}


} // namespace HepMC3

#endif
