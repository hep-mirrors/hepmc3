// -*- C++ -*-
#include "Rivet/Projection.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/BeamConstraint.hh"
#include "Rivet/Event.hh"
#include "Rivet/Cmp.hh"

namespace Rivet {


  Projection::Projection()
    : _name("BaseProjection")
  {
    addPdgIdPair(PID::ANY, PID::ANY);
  }


  Projection:: ~Projection() {
  }


  bool Projection::before(const Projection& p) const {
    const std::type_info& thisid = typeid(*this);
    const std::type_info& otherid = typeid(p);
    if (thisid == otherid) {
      return compare(p) < 0;
    } else {
      return thisid.before(otherid);
    }
  }


  const set<PdgIdPair> Projection::beamPairs() const {
    set<PdgIdPair> ret = _beamPairs;
    set<ConstProjectionPtr> projs = getProjections();
    for (set<ConstProjectionPtr>::const_iterator ip = projs.begin(); ip != projs.end(); ++ip) {
      ConstProjectionPtr p = *ip;
      getLog() << Log::TRACE << "Proj addr = " << p << endl;
      if (p) ret = intersection(ret, p->beamPairs());
    }
    return ret;
  }


  Cmp<Projection> Projection::mkNamedPCmp(const Projection& otherparent,
                                          const string& pname) const {
    return pcmp(*this, otherparent, pname);
  }


  Cmp<Projection> Projection::mkPCmp(const Projection& otherparent,
                                     const string& pname) const {
    return pcmp(*this, otherparent, pname);
  }


}
