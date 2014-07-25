// -*- C++ -*-
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  int VetoedFinalState::compare(const Projection& p) const {
    const PCmp fscmp = mkNamedPCmp(p, "FS");
    if (fscmp != EQUIVALENT) return fscmp;
    if (_vetofsnames.size() != 0) return UNDEFINED;
    const VetoedFinalState& other = dynamic_cast<const VetoedFinalState&>(p);
    return \
      cmp(_vetoCodes, other._vetoCodes) ||
      cmp(_compositeVetoes, other._compositeVetoes) ||
      cmp(_parentVetoes, other._parentVetoes);
  }


  void VetoedFinalState::project(const Event& e) {
    const FinalState& fs = applyProjection<FinalState>(e, "FS");
    _theParticles.clear();
    _theParticles.reserve(fs.particles().size());
    foreach (const Particle& p, fs.particles()) {
      if (getLog().isActive(Log::TRACE)) {
        vector<long> codes;
        for (VetoDetails::const_iterator code = _vetoCodes.begin(); code != _vetoCodes.end(); ++code) {
          codes.push_back(code->first);
        }
        const string codestr = "{ " + join(codes) + " }";
        MSG_TRACE(p.pdgId() << " vs. veto codes = " << codestr << " (" << codes.size() << ")");
      }
      VetoDetails::iterator iter = _vetoCodes.find(p.pdgId());
      if (iter == _vetoCodes.end()) {
        MSG_TRACE("Storing with PDG code = " << p.pdgId() << ", pT = " << p.pT());
        _theParticles.push_back(p);
      } else {
        // This particle code is listed as a possible veto... check pT.
        // Make sure that the pT range is sensible:
        BinaryCut ptrange = iter->second;
        assert(ptrange.first <= ptrange.second);
        stringstream rangess;
        if (ptrange.first < numeric_limits<double>::max()) rangess << ptrange.second;
        rangess << " - ";
        if (ptrange.second < numeric_limits<double>::max()) rangess << ptrange.second;
        MSG_TRACE("ID = " << p.pdgId() << ", pT range = " << rangess.str());
        stringstream debugline;
        debugline << "with PDG code = " << p.pdgId() << " pT = " << p.pT();
        if (p.pT() < ptrange.first || p.pT() > ptrange.second) {
          MSG_TRACE("Storing " << debugline.str());
          _theParticles.push_back(p);
        } else {
          MSG_TRACE("Vetoing " << debugline.str());
        }
      }
    }

    set<Particles::iterator> toErase;
    for (set<int>::iterator nIt = _nCompositeDecays.begin();
         nIt != _nCompositeDecays.end() && !_theParticles.empty(); ++nIt) {
      map<set<Particles::iterator>, FourMomentum> oldMasses;
      map<set<Particles::iterator>, FourMomentum> newMasses;
      set<Particles::iterator> start;
      start.insert(_theParticles.begin());
      oldMasses.insert(pair<set<Particles::iterator>, FourMomentum>
                       (start, _theParticles.begin()->momentum()));

      for (int nParts = 1; nParts != *nIt; ++nParts) {
        for (map<set<Particles::iterator>, FourMomentum>::iterator mIt = oldMasses.begin();
             mIt != oldMasses.end(); ++mIt) {
          Particles::iterator pStart = *(mIt->first.rbegin());
          for (Particles::iterator pIt = pStart + 1; pIt != _theParticles.end(); ++pIt) {
            FourMomentum cMom = mIt->second + pIt->momentum();
            set<Particles::iterator> pList(mIt->first);
            pList.insert(pIt);
            newMasses[pList] = cMom;
          }
        }
        oldMasses = newMasses;
        newMasses.clear();
      }
      for (map<set<Particles::iterator>, FourMomentum>::iterator mIt = oldMasses.begin();
           mIt != oldMasses.end(); ++mIt) {
        double mass2 = mIt->second.mass2();
        if (mass2 >= 0.0) {
          double mass = sqrt(mass2);
          for (CompositeVeto::iterator cIt = _compositeVetoes.lower_bound(*nIt);
               cIt != _compositeVetoes.upper_bound(*nIt); ++cIt) {
            BinaryCut massRange = cIt->second;
            if (mass < massRange.second && mass > massRange.first) {
              for (set<Particles::iterator>::iterator lIt = mIt->first.begin();
                   lIt != mIt->first.end(); ++lIt) {
                toErase.insert(*lIt);
              }
            }
          }
        }
      }
    }

    for (set<Particles::iterator>::reverse_iterator p = toErase.rbegin(); p != toErase.rend(); ++p) {
      _theParticles.erase(*p);
    }

    /// @todo Improve!
    for (ParentVetos::const_iterator vIt = _parentVetoes.begin(); vIt != _parentVetoes.end(); ++vIt) {
      for (Particles::iterator p = _theParticles.begin(); p != _theParticles.end(); ++p) {
        GenVertexPtr startVtx = p->genParticle()->production_vertex();
        bool veto = false;
        if (startVtx) {
          /// @todo Use better HepMC iteration
          for (GenVertex::particle_iterator pIt = startVtx->particles_begin(HepMC3::ancestors);
                   pIt != startVtx->particles_end(HepMC3::ancestors) && !veto; ++pIt) {
            if (*vIt == (*pIt)->pdg_id()) {
              veto = true;
              p = _theParticles.erase(p);
              --p;
            }
          }
        }
      }
    }

    // Now veto on the FS
    foreach (const string& ifs, _vetofsnames) {
      const FinalState& vfs = applyProjection<FinalState>(e, ifs);
      const Particles& vfsp = vfs.particles();
      for (Particles::iterator icheck = _theParticles.begin(); icheck != _theParticles.end(); ++icheck) {
        if ( !icheck->genParticle() ) continue;
        bool found = false;
        for (Particles::const_iterator ipart = vfsp.begin(); ipart != vfsp.end(); ++ipart){
          if ( !ipart->genParticle() ) continue;
          MSG_TRACE("Comparing barcode " << icheck->genParticle()->barcode()
                   << " with veto particle " << ipart->genParticle()->barcode());
          if (ipart->genParticle()->barcode() == icheck->genParticle()->barcode()){
            found = true;
            break;
          }
        }
        if (found) {
          _theParticles.erase(icheck);
          --icheck;
        }
      }
    }
  }


}
