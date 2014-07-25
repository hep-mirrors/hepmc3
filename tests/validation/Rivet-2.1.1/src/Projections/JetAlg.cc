// -*- C++ -*-
#include "Rivet/Projections/JetAlg.hh"

namespace Rivet {


  JetAlg::JetAlg(const FinalState& fs)
    : _useInvisibles(false)
  {
    setName("JetAlg");
    addProjection(fs, "FS");
    VisibleFinalState vfs(fs);
    MSG_DEBUG("Making visible final state from provided FS");
    addProjection(vfs, "VFS");
  }


}
