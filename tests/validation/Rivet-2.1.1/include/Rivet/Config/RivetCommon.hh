#ifndef RIVET_RivetCommon_HH
#define RIVET_RivetCommon_HH

// Convenience build-setup header for Rivet internal use

// Automatic build info from autoconf
#include "Rivet/Config/RivetConfig.hh"
#include "Rivet/Config/BuildOptions.hh"

// Macro to help with overzealous compiler warnings
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#include "Rivet/Exceptions.hh"

#include "Rivet/Tools/RivetSTL.hh"
#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/Tools/RivetHepMC.hh"

#include "Rivet/Tools/Utils.hh"
#include "Rivet/Tools/Logging.hh"

#include "Rivet/Math/Units.hh"
#include "Rivet/ParticleName.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"

#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/Vectors.hh"

// #include "Rivet/Particle.hh"
// #include "Rivet/Event.hh"

#endif
