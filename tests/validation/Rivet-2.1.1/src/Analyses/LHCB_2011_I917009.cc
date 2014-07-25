// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Math/MathUtils.hh"
#include "Rivet/Math/Constants.hh"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/SimpleVector.h"

#include <iostream>
#include <sstream>
#include <string>

namespace Rivet {


  class LHCB_2011_I917009 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    LHCB_2011_I917009()
      : Analysis("LHCB_2011_I917009"),
        rap_beam(0.0), pt_min(0.0),
        pt1_edge(0.65), pt2_edge(1.0),
        pt3_edge(2.5), rap_min(2.),
        rap_max(0.0), dsShift(0)
    {   }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      int y_nbins = 4;
      fillMap(partLftMap);
      if (fuzzyEquals(sqrtS(), 0.9*TeV)) {
        rap_beam = 6.87;
        rap_max = 4.;
        pt_min = 0.25;
      } else if (fuzzyEquals(sqrtS(), 7*TeV)) {
        rap_beam = 8.92;
        rap_max = 4.5;
        pt_min = 0.15;
        y_nbins = 5;
        dsShift = 8;
      } else {
        MSG_ERROR("Incompatible beam energy!");
      }

      // Create the sets of temporary histograms that will be used to make the ratios in the finalize()
      for (size_t i = 0; i < 12; ++i) _tmphistos[i] = YODA::Histo1D(y_nbins, rap_min, rap_max);
      for (size_t i = 12; i < 15; ++i) _tmphistos[i] = YODA::Histo1D(refData(dsShift+5, 1, 1));
      for (size_t i = 15; i < 18; ++i) _tmphistos[i] = YODA::Histo1D(y_nbins, rap_beam - rap_max, rap_beam - rap_min);

      addProjection(UnstableFinalState(), "UFS");
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");
      double ancestor_lftsum = 0.0;
      double y, pT;
      int id;
      int partIdx = -1;
      foreach (const Particle& p, ufs.particles()) {
        id = p.pdgId();
        // continue if particle not a K0s nor (anti-)Lambda
        if ( (id == 310) || (id == -310) )  {
          partIdx = 2;
        } else if ( id == 3122 ) {
          partIdx = 1;
        } else if ( id == -3122 ) {
          partIdx = 0;
        } else {
          continue;
        }
        ancestor_lftsum = getMotherLifeTimeSum(p);
        // Lifetime cut: ctau sum of all particle ancestors < 10^-9 m according to the paper (see eq. 5)
        const double MAX_CTAU = 1.0E-9; // [m]
        if ( (ancestor_lftsum < 0.0) || (ancestor_lftsum > MAX_CTAU) ) continue;
        const FourMomentum& qmom = p.momentum();
        y = log((qmom.E() + qmom.pz())/(qmom.E() - qmom.pz()))/2.;
        // skip this particle if it has too high or too low rapidity (extremely rare cases when E = +- pz)
        if ( std::isnan(y) || std::isinf(y) ) continue;
        y = fabs(y);
        if (!inRange(y, rap_min, rap_max)) continue;
        pT = sqrt((qmom.px() * qmom.px()) + (qmom.py() * qmom.py()));
        if (!inRange(pT, pt_min, pt3_edge)) continue;
        // Filling corresponding temporary histograms for pT intervals
        if (inRange(pT, pt_min, pt1_edge)) _tmphistos[partIdx*3].fill(y, weight);
        if (inRange(pT, pt1_edge, pt2_edge)) _tmphistos[partIdx*3+1].fill(y, weight);
        if (inRange(pT, pt2_edge, pt3_edge)) _tmphistos[partIdx*3+2].fill(y, weight);
        // Fill histo in rapidity for whole pT interval
        _tmphistos[partIdx+9].fill(y, weight);
        // Fill histo in pT for whole rapidity interval
        _tmphistos[partIdx+12].fill(pT, weight);
        // Fill histo in rapidity loss for whole pT interval
        _tmphistos[partIdx+15].fill(rap_beam - y, weight);
      }
    }


    // Generate the ratio histograms
    void finalize() {
      int dsId = dsShift + 1;
      for (size_t j = 0; j < 3; ++j) {
        /// @todo Compactify to two one-liners
        Scatter2DPtr s1 = bookScatter2D(dsId, 1, j+1);
        divide(_tmphistos[j], _tmphistos[3+j], s1);
        Scatter2DPtr s2 = bookScatter2D(dsId+1, 1, j+1);
        divide(_tmphistos[j], _tmphistos[6+j], s2);
      }
      dsId += 2;
      for (size_t j = 3; j < 6; ++j) {
        /// @todo Compactify to two one-liners
        Scatter2DPtr s1 = bookScatter2D(dsId, 1, 1);
        divide(_tmphistos[3*j], _tmphistos[3*j+1], s1);
        dsId += 1;
        Scatter2DPtr s2 = bookScatter2D(dsId, 1, 1);
        divide(_tmphistos[3*j], _tmphistos[3*j+2], s2);
        dsId += 1;
      }
    }

    //@}

  private:


    // Get particle lifetime from hardcoded data
    double getLifeTime(int pid) {
      double lft = -1.0;
      if (pid < 0) pid = - pid;
      // Correct Pythia6 PIDs for f0(980), f0(1370) mesons
      if (pid == 10331) pid = 30221;
      if (pid == 10221) pid = 9010221;
      map<int, double>::iterator pPartLft = partLftMap.find(pid);
      // search stable particle list
      if (pPartLft == partLftMap.end()) {
        if (pid <= 100) return 0.0;
        for (size_t i=0; i < sizeof(stablePDGIds)/sizeof(unsigned int); i++) {
          if (pid == stablePDGIds[i]) { lft = 0.0; break; }
        }
      } else {
        lft = (*pPartLft).second;
      }
      if (lft < 0.0 && PID::isHadron(pid)) {
        MSG_ERROR("Could not determine lifetime for particle with PID " << pid
                  << "... This V^0 will be considered unprompt!");
      }
      return lft;
    }

    // Data members like post-cuts event weight counters go here
    const double getMotherLifeTimeSum(const Particle& p) {
      if (p.genParticle() == NULL) return -1.;
      double lftSum = 0.;
      double plft = 0.;
      const GenParticle* part = p.genParticle();
      GenVertex* ivtx = part->production_vertex();
      while(ivtx)
        {
          if (ivtx->particles_in_size() < 1) { lftSum = -1.; break; };
          const HepMC::GenVertex::particles_in_const_iterator iPart_invtx = ivtx->particles_in_const_begin();
          part = (*iPart_invtx);
          if ( !(part) ) { lftSum = -1.; break; };
          ivtx = part->production_vertex();
          if ( (part->pdg_id() == 2212) || !(ivtx) ) break; //reached beam
          plft = getLifeTime(part->pdg_id());
          if (plft < 0.) { lftSum = -1.; break; };
          lftSum += plft;
        };
      return (lftSum * c_light);
    }

    /// @name Private variables
    //@{

    // The rapidity of the beam according to the selected beam energy
    double rap_beam;

    // The edges of the intervals of transverse momentum
    double pt_min, pt1_edge, pt2_edge, pt3_edge;

    // The limits of the rapidity window
    double rap_min;
    double rap_max;

    // Indicates which set of histograms will be output to yoda file (according to beam energy)
    int dsShift;

    // Map between PDG id and particle lifetimes in seconds
    std::map<int, double> partLftMap;

    // Set of PDG Ids for stable particles (PDG Id <= 100 are considered stable)
    static const int stablePDGIds[205];

    //@}

    /// @name Helper histograms
    //@{
    /// Histograms are defined in the following order: anti-Lambda, Lambda and K0s.
    /// First 3 suites of 3 histograms correspond to each particle in bins of y for the 3 pT intervals. (9 histos)
    /// Next 3 histograms contain the particles in y bins for the whole pT interval (3 histos)
    /// Next 3 histograms contain the particles in y_loss bins for the whole pT interval (3 histos)
    /// Last 3 histograms contain the particles in pT bins for the whole rapidity (y) interval (3 histos)
    YODA::Histo1D _tmphistos[18];
    //@}

    // Fill the PDG Id to Lifetime[seconds] map
    // Data was extracted from LHCb Particle Table through LHCb::ParticlePropertySvc
    bool fillMap(map<int, double>& m) {
      m[6] =  4.707703E-25;  m[11] =  1.E+16;  m[12] =  1.E+16;
      m[13] =  2.197019E-06;  m[14] =  1.E+16;  m[15] =  2.906E-13;  m[16] =  1.E+16;  m[22] =  1.E+16;
      m[23] =  2.637914E-25;  m[24] =  3.075758E-25;  m[25] =  9.4E-26;  m[35] =  9.4E-26;
      m[36] =  9.4E-26;  m[37] =  9.4E-26;  m[84] =  3.335641E-13;  m[85] =  1.290893E-12;
      m[111] =  8.4E-17;  m[113] =  4.405704E-24;  m[115] =  6.151516E-24;  m[117] =  4.088275E-24;
      m[119] =  2.102914E-24;  m[130] =  5.116E-08;  m[150] =  1.525E-12;  m[211] =  2.6033E-08;
      m[213] =  4.405704E-24;  m[215] =  6.151516E-24;  m[217] =  4.088275E-24;  m[219] =  2.102914E-24;
      m[221] =  5.063171E-19;  m[223] =  7.752794E-23;  m[225] =  3.555982E-24;  m[227] =  3.91793E-24;
      m[229] =  2.777267E-24;  m[310] =  8.953E-11;  m[313] =  1.308573E-23;  m[315] =  6.038644E-24;
      m[317] =  4.139699E-24;  m[319] =  3.324304E-24;  m[321] =  1.238E-08;  m[323] =  1.295693E-23;
      m[325] =  6.682357E-24;  m[327] =  4.139699E-24;  m[329] =  3.324304E-24;  m[331] =  3.210791E-21;
      m[333] =  1.545099E-22;  m[335] =  9.016605E-24;  m[337] =  7.565657E-24;  m[350] =  1.407125E-12;
      m[411] =  1.04E-12;  m[413] =  6.856377E-21;  m[415] =  1.778952E-23;  m[421] =  4.101E-13;
      m[423] =  1.000003E-19;  m[425] =  1.530726E-23;  m[431] =  5.E-13;  m[433] =  1.000003E-19;
      m[435] =  3.291061E-23;  m[441] =  2.465214E-23;  m[443] =  7.062363E-21;  m[445] =  3.242425E-22;
      m[510] =  1.525E-12;  m[511] =  1.525E-12;  m[513] =  1.000019E-19;  m[515] =  1.31E-23;
      m[521] =  1.638E-12;  m[523] =  1.000019E-19;  m[525] =  1.31E-23;  m[530] =  1.536875E-12;
      m[531] =  1.472E-12;  m[533] =  1.E-19;  m[535] =  1.31E-23;  m[541] =  4.5E-13;
      m[553] =  1.218911E-20;  m[1112] =  4.539394E-24;  m[1114] =  5.578069E-24;  m[1116] =  1.994582E-24;
      m[1118] =  2.269697E-24;  m[1212] =  4.539394E-24;  m[1214] =  5.723584E-24;  m[1216] =  1.994582E-24;
      m[1218] =  1.316424E-24;  m[2112] =  8.857E+02;  m[2114] =  5.578069E-24;  m[2116] =  4.388081E-24;
      m[2118] =  2.269697E-24;  m[2122] =  4.539394E-24;  m[2124] =  5.723584E-24;  m[2126] =  1.994582E-24;
      m[2128] =  1.316424E-24;  m[2212] =  1.E+16;  m[2214] =  5.578069E-24;  m[2216] =  4.388081E-24;
      m[2218] =  2.269697E-24;  m[2222] =  4.539394E-24;  m[2224] =  5.578069E-24;  m[2226] =  1.994582E-24;
      m[2228] =  2.269697E-24;  m[3112] =  1.479E-10;  m[3114] =  1.670589E-23;  m[3116] =  5.485102E-24;
      m[3118] =  3.656734E-24;  m[3122] =  2.631E-10;  m[3124] =  4.219309E-23;  m[3126] =  8.227653E-24;
      m[3128] =  3.291061E-24;  m[3212] =  7.4E-20;  m[3214] =  1.828367E-23;  m[3216] =  5.485102E-24;
      m[3218] =  3.656734E-24;  m[3222] =  8.018E-11;  m[3224] =  1.838582E-23;  m[3226] =  5.485102E-24;
      m[3228] =  3.656734E-24;  m[3312] =  1.639E-10;  m[3314] =  6.648608E-23;  m[3322] =  2.9E-10;
      m[3324] =  7.233101E-23;  m[3334] =  8.21E-11;  m[4112] =  2.991874E-22;  m[4114] =  4.088274E-23;
      m[4122] =  2.E-13;  m[4132] =  1.12E-13;  m[4212] =  3.999999E-22;  m[4214] =  3.291061E-22;
      m[4222] =  2.951624E-22;  m[4224] =  4.417531E-23;  m[4232] =  4.42E-13;  m[4332] =  6.9E-14;
      m[4412] =  3.335641E-13;  m[4422] =  3.335641E-13;  m[4432] =  3.335641E-13;  m[5112] =  1.E-19;
      m[5122] =  1.38E-12;  m[5132] =  1.42E-12;  m[5142] =  1.290893E-12;  m[5212] =  1.E-19;
      m[5222] =  1.E-19;  m[5232] =  1.42E-12;  m[5242] =  1.290893E-12;  m[5312] =  1.E-19;
      m[5322] =  1.E-19;  m[5332] =  1.55E-12;  m[5342] =  1.290893E-12;  m[5442] =  1.290893E-12;
      m[5512] =  1.290893E-12;  m[5522] =  1.290893E-12;  m[5532] =  1.290893E-12;  m[5542] =  1.290893E-12;
      m[10111] =  2.48382E-24;  m[10113] =  4.635297E-24;  m[10115] =  2.54136E-24;  m[10211] =  2.48382E-24;
      m[10213] =  4.635297E-24;  m[10215] =  2.54136E-24;  m[10223] =  1.828367E-24;  m[10225] =  3.636531E-24;
      m[10311] =  2.437823E-24;  m[10313] =  7.313469E-24;  m[10315] =  3.538775E-24;
      m[10321] =  2.437823E-24;  m[10323] =  7.313469E-24;  m[10325] =  3.538775E-24;
      m[10331] =  4.804469E-24;  m[10411] =  4.38E-24;  m[10413] =  3.29E-23;  m[10421] =  4.38E-24;
      m[10423] =  3.22653E-23;  m[10431] =  6.5821E-22;  m[10433] =  6.5821E-22;  m[10441] =  6.453061E-23;
      m[10511] =  4.39E-24;  m[10513] =  1.65E-23;  m[10521] =  4.39E-24;  m[10523] =  1.65E-23;
      m[10531] =  4.39E-24;  m[10533] =  1.65E-23;  m[11114] =  2.194041E-24;  m[11116] =  1.828367E-24;
      m[11212] =  1.880606E-24;  m[11216] =  1.828367E-24;  m[12112] =  2.194041E-24;
      m[12114] =  2.194041E-24;  m[12116] =  5.063171E-24;  m[12126] =  1.828367E-24;
      m[12212] =  2.194041E-24;  m[12214] =  2.194041E-24;  m[12216] =  5.063171E-24;
      m[12224] =  2.194041E-24;  m[12226] =  1.828367E-24;  m[13112] =  6.582122E-24;  m[13114] =  1.09702E-23;
      m[13116] =  5.485102E-24;  m[13122] =  1.316424E-23;  m[13124] =  1.09702E-23;  m[13126] =  6.928549E-24;
      m[13212] =  6.582122E-24;  m[13214] =  1.09702E-23;  m[13216] =  5.485102E-24;  m[13222] =  6.582122E-24;
      m[13224] =  1.09702E-23;  m[13226] =  5.485102E-24;  m[13314] =  2.742551E-23;
      m[13324] =  2.742551E-23;  m[14122] =  1.828367E-22;  m[20022] =  1.E+16;  m[20113] =  1.567172E-24;
      m[20213] =  1.567172E-24;  m[20223] =  2.708692E-23;  m[20313] =  3.782829E-24;
      m[20315] =  2.384827E-24;  m[20323] =  3.782829E-24;  m[20325] =  2.384827E-24;
      m[20333] =  1.198929E-23;  m[20413] =  2.63E-24;  m[20423] =  2.63E-24;  m[20433] =  6.5821E-22;
      m[20443] =  7.395643E-22;  m[20513] =  2.63E-24;  m[20523] =  2.63E-24;  m[20533] =  2.63E-24;
      m[21112] =  2.632849E-24;  m[21114] =  3.291061E-24;  m[21212] =  2.632849E-24;
      m[21214] =  6.582122E-24;  m[22112] =  4.388081E-24;  m[22114] =  3.291061E-24;
      m[22122] =  2.632849E-24;  m[22124] =  6.582122E-24;  m[22212] =  4.388081E-24;
      m[22214] =  3.291061E-24;  m[22222] =  2.632849E-24;  m[22224] =  3.291061E-24;
      m[23112] =  7.313469E-24;  m[23114] =  2.991874E-24;  m[23122] =  4.388081E-24;
      m[23124] =  6.582122E-24;  m[23126] =  3.291061E-24;  m[23212] =  7.313469E-24;
      m[23214] =  2.991874E-24;  m[23222] =  7.313469E-24;  m[23224] =  2.991874E-24;
      m[30113] =  2.632849E-24;  m[30213] =  2.632849E-24;  m[30221] =  1.880606E-24;
      m[30223] =  2.089563E-24;  m[30313] =  2.056913E-24;  m[30323] =  2.056913E-24;
      m[30443] =  2.419898E-23;  m[31114] =  1.880606E-24;  m[31214] =  3.291061E-24;
      m[32112] =  3.989164E-24;  m[32114] =  1.880606E-24;  m[32124] =  3.291061E-24;
      m[32212] =  3.989164E-24;  m[32214] =  1.880606E-24;  m[32224] =  1.880606E-24;
      m[33122] =  1.880606E-23;  m[42112] =  6.582122E-24;  m[42212] =  6.582122E-24;
      m[43122] =  2.194041E-24;  m[53122] =  4.388081E-24;  m[100111] =  1.645531E-24;
      m[100113] =  1.64553E-24;  m[100211] =  1.645531E-24;  m[100213] =  1.64553E-24;
      m[100221] =  1.196749E-23;  m[100223] =  3.061452E-24;  m[100313] =  2.837122E-24;
      m[100323] =  2.837122E-24;  m[100331] =  4.459432E-25;  m[100333] =  4.388081E-24;
      m[100441] =  4.701516E-23;  m[100443] =  2.076379E-21;  m[100553] =  2.056913E-20;
      m[200553] =  3.242425E-20;  m[300553] =  3.210791E-23;  m[9000111] =  8.776163E-24;
      m[9000211] =  8.776163E-24;  m[9000443] =  8.227652E-24;  m[9000553] =  5.983747E-24;
      m[9010111] =  3.164482E-24;  m[9010211] =  3.164482E-24;  m[9010221] =  9.403031E-24;
      m[9010443] =  8.438618E-24;  m[9010553] =  8.3318E-24;  m[9020443] =  1.061633E-23;
      m[9030221] =  6.038644E-24;  m[9042413] =  2.07634E-21;  m[9050225] =  1.394517E-24;
      m[9060225] =  3.291061E-24;  m[9080225] =  4.388081E-24;  m[9090225] =  2.056913E-24;
      m[9910445] =  2.07634E-21;  m[9920443] =  2.07634E-21;
      return true;
    }

  };


  const int LHCB_2011_I917009::stablePDGIds[205] = {
    311, 543, 545, 551, 555, 557, 1103, 2101, 2103, 2203, 3101, 3103, 3201, 3203, 3303,
    4101, 4103, 4124, 4201, 4203, 4301, 4303, 4312, 4314, 4322, 4324, 4334, 4403, 4414,
    4424, 4434, 4444, 5101, 5103, 5114, 5201, 5203, 5214, 5224, 5301, 5303, 5314, 5324,
    5334, 5401, 5403, 5412, 5414, 5422, 5424, 5432, 5434, 5444, 5503, 5514, 5524, 5534,
    5544, 5554, 10022, 10333, 10335, 10443, 10541, 10543, 10551, 10553, 10555, 11112,
    12118, 12122, 12218, 12222, 13316, 13326, 20543, 20553, 20555, 23314, 23324, 30343,
    30353, 30363, 30553, 33314, 33324, 41214, 42124, 52114, 52214, 100311, 100315, 100321,
    100325, 100411, 100413, 100421, 100423, 100551, 100555, 100557, 110551, 110553, 110555,
    120553, 120555, 130553, 200551, 200555, 210551, 210553, 220553, 1000001, 1000002,
    1000003, 1000004, 1000005, 1000006, 1000011, 1000012, 1000013, 1000014, 1000015,
    1000016, 1000021, 1000022, 1000023, 1000024, 1000025, 1000035, 1000037, 1000039,
    2000001, 2000002, 2000003, 2000004, 2000005, 2000006, 2000011, 2000012, 2000013,
    2000014, 2000015, 2000016, 3000111, 3000113, 3000211, 3000213, 3000221, 3000223,
    3000331, 3100021, 3100111, 3100113, 3200111, 3200113, 3300113, 3400113, 4000001,
    4000002, 4000011, 4000012, 5000039, 9000221, 9900012, 9900014, 9900016, 9900023,
    9900024, 9900041, 9900042 };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(LHCB_2011_I917009);

}
