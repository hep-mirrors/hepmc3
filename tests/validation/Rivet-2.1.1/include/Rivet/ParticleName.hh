#ifndef RIVET_PARTICLENAME_HH
#define RIVET_PARTICLENAME_HH

#include "Rivet/Particle.fhh"
#include "Rivet/Exceptions.hh"

namespace Rivet {

  namespace PID {


    /// Static const convenience particle ID names

    /// Special wildcard particle name
    static const PdgId ANY = 10000;

    /// @name Charged leptons
    //@{
    static const PdgId ELECTRON = 11;
    static const PdgId POSITRON = -ELECTRON;
    static const PdgId EMINUS = ELECTRON;
    static const PdgId EPLUS = POSITRON;
    static const PdgId MUON = 13;
    static const PdgId ANTIMUON = -MUON;
    static const PdgId TAU = 15;
    static const PdgId ANTITAU = -TAU;
    //@}

    /// @name Neutrinos
    //@{
    static const PdgId NU_E = 12;
    static const PdgId NU_EBAR = -NU_E;
    static const PdgId NU_MU = 14;
    static const PdgId NU_MUBAR = -NU_MU;
    static const PdgId NU_TAU = 16;
    static const PdgId NU_TAUBAR = -NU_TAU;
    //@}

    /// @name Bosons
    //@{
    static const PdgId PHOTON = 22;
    static const PdgId GAMMA = PHOTON;
    static const PdgId GLUON = 21;
    static const PdgId WPLUSBOSON = 24;
    static const PdgId WMINUSBOSON = -WPLUSBOSON;
    static const PdgId WPLUS = WPLUSBOSON;
    static const PdgId WMINUS = WMINUSBOSON;
    static const PdgId Z0BOSON = 23;
    static const PdgId ZBOSON = Z0BOSON;
    static const PdgId Z0 = Z0BOSON;
    static const PdgId HIGGSBOSON = 25;
    static const PdgId HIGGS = HIGGSBOSON;
    //@}

    /// @name Quarks
    //@{
    static const PdgId DQUARK = 1;
    static const PdgId UQUARK = 2;
    static const PdgId SQUARK = 3;
    static const PdgId CQUARK = 4;
    static const PdgId BQUARK = 5;
    static const PdgId TQUARK = 6;
    //@}

    /// @name Nucleons
    //@{
    static const PdgId PROTON = 2212;
    static const PdgId ANTIPROTON = -PROTON;
    static const PdgId PBAR = ANTIPROTON;
    static const PdgId NEUTRON = 2112;
    static const PdgId ANTINEUTRON = -NEUTRON;
    //@}

    /// @name Light mesons
    //@{
    static const PdgId PI0 = 111;
    static const PdgId PIPLUS = 211;
    static const PdgId PIMINUS = -PIPLUS;
    static const PdgId K0L = 130;
    static const PdgId K0S = 310;
    static const PdgId KPLUS = 321;
    static const PdgId KMINUS = -KPLUS;
    static const PdgId ETA = 221;
    static const PdgId ETAPRIME = 331;
    static const PdgId PHI = 333;
    static const PdgId OMEGA = 223;
    //@}

    /// @name Charmonia
    //@{
    static const PdgId ETAC = 441;
    static const PdgId JPSI = 443;
    static const PdgId PSI2S = 100443;
    //@}

    /// @name Charm mesons
    //@{
    static const PdgId D0 = 421;
    static const PdgId DPLUS = 411;
    static const PdgId DMINUS = -DPLUS;
    static const PdgId DSPLUS = 431;
    static const PdgId DSMINUS = -DSPLUS;
    //@}

    /// @name Bottomonia
    //@{
    static const PdgId ETAB = 551;
    static const PdgId UPSILON1S = 553;
    static const PdgId UPSILON2S = 100553;
    static const PdgId UPSILON3S = 200553;
    static const PdgId UPSILON4S = 300553;
    //@}

    /// @name b mesons
    //@{
    static const PdgId B0 = 511;
    static const PdgId BPLUS = 521;
    static const PdgId BMINUS = -BPLUS;
    static const PdgId B0S = 531;
    static const PdgId BCPLUS = 541;
    static const PdgId BCMINUS = -BCPLUS;
    //@}

    /// @name Baryons
    //@{
    static const PdgId LAMBDA = 3122;
    static const PdgId SIGMA0 = 3212;
    static const PdgId SIGMAPLUS = 3222;
    static const PdgId SIGMAMINUS = 3112;
    static const PdgId LAMBDACPLUS = 4122;
    static const PdgId LAMBDACMINUS = 4122;
    static const PdgId LAMBDAB = 5122;
    static const PdgId XI0 = 3322;
    static const PdgId XIMINUS = 3312;
    static const PdgId XIPLUS = -XIMINUS;
    static const PdgId OMEGAMINUS = 3334;
    static const PdgId OMEGAPLUS = -OMEGAMINUS;
    //@}

    /// @name Exotic/weird stuff
    //@{
    static const PdgId REGGEON = 110;
    static const PdgId POMERON = 990;
    static const PdgId ODDERON = 9990;
    static const PdgId GRAVITON = 39;
    static const PdgId NEUTRALINO1 = 1000022;
    static const PdgId GRAVITINO = 1000039;
    static const PdgId GLUINO = 1000021;
    /// @todo Add axion, black hole remnant, etc. on demand
    //@}



    /// Handler for particle name code <-> string conversion
    ///
    /// @todo Hide this utility class or equiv in an unnamed namespace in a .cc file.
    class ParticleNames {
    public:

      static const std::string& particleName(PdgId pid) {
        /// @todo Isn't there a nicer, pointerless way to do singletons?
        if (!_instance) _instance = new ParticleNames();
        return _instance->_particleName(pid);
      }

      static PdgId particleId(const std::string& pname) {
        /// @todo Isn't there a nicer, pointerless way to do singletons?
        if (!_instance) _instance = new ParticleNames();
        return _instance->_particleId(pname);
      }

      const std::string& _particleName(PdgId pid);

      PdgId _particleId(const std::string& pname);


    private:

      ParticleNames() {
        _add_pid_name(ELECTRON, "ELECTRON");
        _add_pid_name(POSITRON, "POSITRON");
        _add_pid_name(PROTON, "PROTON");
        _add_pid_name(ANTIPROTON, "ANTIPROTON");
        _add_pid_name(PHOTON, "PHOTON");
        _add_pid_name(NEUTRON, "NEUTRON");
        _add_pid_name(ANTINEUTRON, "ANTINEUTRON");
        _add_pid_name(MUON, "MUON");
        _add_pid_name(ANTIMUON, "ANTIMUON");
        _add_pid_name(NU_E, "NU_E");
        _add_pid_name(NU_EBAR, "NU_EBAR");
        _add_pid_name(NU_MU, "NU_MU");
        _add_pid_name(NU_MUBAR, "NU_MUBAR");
        _add_pid_name(NU_TAU, "NU_TAU");
        _add_pid_name(NU_TAUBAR, "NU_TAUBAR");
        _add_pid_name(PIPLUS, "PIPLUS");
        _add_pid_name(PIMINUS, "PIMINUS");
        _add_pid_name(TAU, "TAU");
        _add_pid_name(WPLUSBOSON, "WPLUSBOSON");
        _add_pid_name(WMINUSBOSON, "WMINUSBOSON");
        _add_pid_name(ZBOSON, "ZBOSON");
        _add_pid_name(HIGGS, "HIGGS");
        _add_pid_name(ANTITAU, "ANTITAU");
        _add_pid_name(ANY, "*");
      }

      void _add_pid_name(PdgId pid, const std::string& pname) {
        _ids_names[pid] = pname;
        _names_ids[pname] = pid;
      }

      static ParticleNames* _instance;

      std::map<PdgId, std::string> _ids_names;

      std::map<std::string, PdgId> _names_ids;

    };


    /// Print a PdgId as a named string.
    inline const std::string& toParticleName(PdgId p) {
      return ParticleNames::particleName(p);
    }


    /// Print a PdgId as a named string.
    inline PdgId toParticleId(const std::string& pname) {
      return ParticleNames::particleId(pname);
    }


    /// Convenience maker of particle ID pairs from PdgIds.
    inline std::pair<PdgId,PdgId> make_pdgid_pair(PdgId a, PdgId b) {
      return make_pair(a, b);
    }


    /// Convenience maker of particle ID pairs from particle names.
    inline std::pair<PdgId,PdgId> make_pdgid_pair(const std::string& a, const std::string& b) {
      const PdgId pa = toParticleId(a);
      const PdgId pb = toParticleId(b);
      return make_pair(pa, pb);
    }


    /// Print a PdgIdPair as a string.
    inline std::string toBeamsString(const PdgIdPair& pair) {
      string out = "[" +
        toParticleName(pair.first) + ", " +
        toParticleName(pair.second) + "]";
      return out;
    }


  }

}

#endif
