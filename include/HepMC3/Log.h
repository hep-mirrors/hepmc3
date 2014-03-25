#ifndef  HEPMC3_LOG_H
#define  HEPMC3_LOG_H
/**
 * @brief class for logging debug and error messages
 *
 * @author Tomasz Przedzinski
 * @date 25th March 2014
 */

#include <iostream>

namespace HepMC3 {

class Log {
//
// Constructors
//
public:

//
// Accessors
//
public:
    static bool print_errors()                { return m_is_printing_errors;    }
    static bool print_warnings()              { return m_is_printing_warnings;  }
    static int  debug_level()                 { return m_debug_level;           }
    static void set_print_errors(bool flag)   { m_is_printing_errors   = flag;  }
    static void set_print_warnings(bool flag) { m_is_printing_warnings = flag;  }
    static void set_debug_level(int level)    { m_debug_level          = level; }

private:
    static bool m_is_printing_errors;
    static bool m_is_printing_warnings;
    static int  m_debug_level;
};

#define ERROR(MESSAGE)       if( HepMC3::Log::print_errors() )         { std::cerr << "ERROR::HepMC3::"                 << MESSAGE << std::endl; }
#define WARNING(MESSAGE)     if( HepMC3::Log::print_warnings() )       { std::cout << "WARNING::HepMC3::"               << MESSAGE << std::endl; }
#define DEBUG(LEVEL,MESSAGE) if( HepMC3::Log::debug_level()>=(LEVEL) ) { std::cout << "DEBUG(" << LEVEL <<")::HepMC3::" << MESSAGE << std::endl; }

} // namespace HepMC3

#endif
