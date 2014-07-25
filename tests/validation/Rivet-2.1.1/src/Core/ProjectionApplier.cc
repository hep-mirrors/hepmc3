// -*- C++ -*-
#include "Rivet/ProjectionApplier.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  // NB. Allow proj registration in constructor by default -- explicitly disable for Analysis
  ProjectionApplier::ProjectionApplier()
    : _allowProjReg(true),
      _projhandler(ProjectionHandler::getInstance())
  {  }


  ProjectionApplier::~ProjectionApplier() {
    getProjHandler().removeProjectionApplier(*this);
  }


  const Projection& ProjectionApplier::_applyProjection(const Event& evt,
                                                        const string& name) const {
    return evt.applyProjection(getProjection(name));
  }


  const Projection& ProjectionApplier::_applyProjection(const Event& evt,
                                                        const Projection& proj) const {
    return evt.applyProjection(proj);
  }


  const Projection& ProjectionApplier::_addProjection(const Projection& proj,
                                                      const std::string& name) {
    if (!_allowProjReg) {
      cerr << "Trying to register projection '"
           << proj.name() << "' before init phase in '" << this->name() << "'." << endl;
      exit(2);
    }
    const Projection& reg = getProjHandler().registerProjection(*this, proj, name);
    return reg;
  }


}
