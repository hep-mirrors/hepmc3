#include "HepMC3/Setup.h"
/**
 *  @file Setup.cc
 *  @brief Initialization of static fields of HepMC3::Setup class
 *
 */
namespace HepMC3 {

bool Setup::m_is_printing_errors    = true;
bool Setup::m_is_printing_warnings  = true;
int  Setup::m_debug_level           = 5;

} // namespace HepMC3
