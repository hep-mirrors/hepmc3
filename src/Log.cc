#include "HepMC3/Log.h"
/**
 *  @file Log.cc
 *  @brief Initialization of static fields of HepMC3::Log class
 *
 */
namespace HepMC3 {

bool Log::m_is_printing_errors    = true;
bool Log::m_is_printing_warnings  = true;
int  Log::m_debug_level           = 5;

} // namespace HepMC3
