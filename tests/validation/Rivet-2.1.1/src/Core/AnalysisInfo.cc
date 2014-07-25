#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/AnalysisInfo.hh"
#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/Tools/RivetPaths.hh"
#include "Rivet/Tools/Utils.hh"
#include "Rivet/Tools/Logging.hh"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

#ifdef YAML_NAMESPACE
#define YAML YAML_NAMESPACE
#endif

namespace Rivet {


  namespace {
    Log& getLog() {
      return Log::getLog("Rivet.AnalysisInfo");
    }
  }


  /// Static factory method
  AnalysisInfo* AnalysisInfo::make(const std::string& ananame) {
    // Returned AI, in semi-null state
    AnalysisInfo* ai = new AnalysisInfo();
    ai->_beams += make_pair(PID::ANY, PID::ANY);
    ai->_name = ananame;

    /// If no ana data file found, return null AI
    const string datapath = findAnalysisInfoFile(ananame + ".info");
    if (datapath.empty()) {
      MSG_DEBUG("No datafile " << ananame + ".info found");
      return ai;
    }

    // Read data from YAML document
    MSG_DEBUG("Reading analysis data from " << datapath);
    YAML::Node doc;
    try {
      #if YAMLCPP_API == 3
      std::ifstream file(datapath.c_str());
      YAML::Parser parser(file);
      parser.GetNextDocument(doc);
      #elif YAMLCPP_API == 5
      doc = YAML::LoadFile(datapath);
      #endif
    } catch (const YAML::ParserException& ex) {
      MSG_ERROR("Parse error when reading analysis data from " << datapath << " (" << ex.what() << ")");
      return ai;
    }

    #define THROW_INFOERR(KEY) throw InfoError("Problem in info parsing while accessing key " + string(KEY) + " in file " + datapath)

    // Simple scalars (test for nullness before casting)
    #if YAMLCPP_API == 3
    #define TRY_GETINFO(KEY, VAR) try { if (doc.FindValue(KEY)) { string val; doc[KEY] >> val; ai->_ ## VAR = val; } } catch (...) { THROW_INFOERR(KEY); }
    #elif YAMLCPP_API == 5
    #define TRY_GETINFO(KEY, VAR) try { if (doc[KEY] && !doc[KEY].IsNull()) ai->_ ## VAR = doc[KEY].as<string>(); } catch (...) { THROW_INFOERR(KEY); }
    #endif
    TRY_GETINFO("Name", name);
    TRY_GETINFO("Summary", summary);
    TRY_GETINFO("Status", status);
    TRY_GETINFO("RunInfo", runInfo);
    TRY_GETINFO("Description", description);
    TRY_GETINFO("Experiment", experiment);
    TRY_GETINFO("Collider", collider);
    TRY_GETINFO("Year", year);
    TRY_GETINFO("SpiresID", spiresId);
    TRY_GETINFO("InspireID", inspireId);
    TRY_GETINFO("BibKey", bibKey);
    TRY_GETINFO("BibTeX", bibTeX);
    #undef TRY_GETINFO

    // Sequences (test the seq *and* each entry for nullness before casting)
    #if YAMLCPP_API == 3
    #define TRY_GETINFO_SEQ(KEY, VAR) try { \
        if (const YAML::Node* VAR = doc.FindValue(KEY)) {               \
          for (size_t i = 0; i < VAR->size(); ++i) {                    \
            string val; (*VAR)[i] >> val; ai->_ ## VAR += val;          \
          } } } catch (...) { THROW_INFOERR(KEY); }
    #elif YAMLCPP_API == 5
    #define TRY_GETINFO_SEQ(KEY, VAR) try { \
        if (doc[KEY] && !doc[KEY].IsNull()) {                           \
          const YAML::Node& VAR = doc[KEY];                             \
          for (size_t i = 0; i < VAR.size(); ++i)                       \
            if (!VAR[i].IsNull()) ai->_ ## VAR += VAR[i].as<string>();  \
        } } catch (...) { THROW_INFOERR(KEY); }
    #endif
    TRY_GETINFO_SEQ("Authors", authors);
    TRY_GETINFO_SEQ("References", references);
    TRY_GETINFO_SEQ("ToDo", todos);
    #undef TRY_GETINFO_SEQ


    // A boolean with some name flexibility
    try {
      #if YAMLCPP_API == 3
      bool val;
      if (const YAML::Node* n = doc.FindValue("NeedsCrossSection")) { *n >> val; ai->_needsCrossSection = val; }
      if (const YAML::Node* n = doc.FindValue("NeedCrossSection")) { *n >> val; ai->_needsCrossSection = val; }
      #elif YAMLCPP_API == 5
      if (doc["NeedsCrossSection"]) ai->_needsCrossSection = doc["NeedsCrossSection"].as<bool>();
      else if (doc["NeedCrossSection"]) ai->_needsCrossSection = doc["NeedCrossSection"].as<bool>();
      #endif
    } catch (...) {
      THROW_INFOERR("NeedsCrossSection|NeedCrossSection");
    }


    // Beam particle identities
    try {
      #if YAMLCPP_API == 3

      if (const YAML::Node* pbeampairs = doc.FindValue("Beams")) {
        const YAML::Node& beampairs = *pbeampairs;
        vector<PdgIdPair> beam_pairs;
        if (beampairs.size() == 2 &&
            beampairs[0].Type() == YAML::NodeType::Scalar &&
            beampairs[1].Type() == YAML::NodeType::Scalar) {
          string bstr0, bstr1;
          beampairs[0] >> bstr0;
          beampairs[1] >> bstr1;
          beam_pairs += PID::make_pdgid_pair(bstr0, bstr1);
        } else {
          for (YAML::Iterator bpi = beampairs.begin(); bpi != beampairs.end(); ++bpi) {
            const YAML::Node& bp = *bpi;
            if (bp.size() == 2 &&
                bp[0].Type() == YAML::NodeType::Scalar &&
                bp[1].Type() == YAML::NodeType::Scalar) {
              string bstr0, bstr1;
              bp[0] >> bstr0;
              bp[1] >> bstr1;
              beam_pairs += PID::make_pdgid_pair(bstr0, bstr1);
            } else {
              throw InfoError("Beam ID pairs have to be either a 2-tuple or a list of 2-tuples of particle names");
            }
          }
        }
        ai->_beams = beam_pairs;
      }

      #elif YAMLCPP_API == 5

      if (doc["Beams"]) {
        const YAML::Node& beams = doc["Beams"];
        vector<PdgIdPair> beam_pairs;
        if (beams.size() == 2 && beams[0].IsScalar() && beams[0].IsScalar()) {
          beam_pairs += PID::make_pdgid_pair(beams[0].as<string>(), beams[1].as<string>());
        } else {
          for (size_t i = 0; i < beams.size(); ++i) {
            const YAML::Node& bp = beams[i];
            if (bp.size() != 2 || !bp[0].IsScalar() || !bp[0].IsScalar())
              throw InfoError("Beam ID pairs have to be either a 2-tuple or a list of 2-tuples of particle names");
            beam_pairs += PID::make_pdgid_pair(bp[0].as<string>(), bp[1].as<string>());
          }
        }
        ai->_beams = beam_pairs;
      }

      #endif
    } catch (...) { THROW_INFOERR("Beams"); }


    // Beam energies
    try {
      #if YAMLCPP_API == 3

      if (const YAML::Node* penergies = doc.FindValue("Energies")) {
        const YAML::Node& energies = *penergies;
        vector<pair<double,double> > beam_energy_pairs;
        for (YAML::Iterator be = energies.begin(); be != energies.end(); ++be) {
          if (be->Type() == YAML::NodeType::Scalar) {
            // If beam energy is a scalar, then assume symmetric beams each with half that energy
            double sqrts;
            *be >> sqrts;
            beam_energy_pairs += make_pair(sqrts/2.0, sqrts/2.0);
          } else if (be->Type() == YAML::NodeType::Sequence) {
            const YAML::Node& beseq = *be;
            // If the sub-sequence is of length 1, then it's another scalar sqrt(s)!
            if (beseq.size() == 1) {
              double sqrts;
              (*be)[0] >> sqrts;
              beam_energy_pairs += make_pair(sqrts/2.0, sqrts/2.0);
            } else if (beseq.size() == 2) {
              vector<double> beamenergies;
              double beamenergy0, beamenergy1;
              beseq[0] >> beamenergy0;
              beseq[1] >> beamenergy1;
              beam_energy_pairs += make_pair(beamenergy0, beamenergy1);
            } else {
              throw InfoError("Beam energies have to be a list of either numbers or pairs of numbers");
            }
          } else {
            throw InfoError("Beam energies have to be a list of either numbers or pairs of numbers");
          }
        }
        ai->_energies = beam_energy_pairs;
      }

      #elif YAMLCPP_API == 5

      if (doc["Energies"]) {
        vector< pair<double,double> > beam_energy_pairs;
        for (size_t i = 0; i < doc["Energies"].size(); ++i) {
          const YAML::Node& be = doc["Energies"][i];
          if (be.IsScalar()) {
            // If beam energy is a scalar, then assume symmetric beams each with half that energy
            beam_energy_pairs += make_pair(be.as<double>()/2.0, be.as<double>()/2.0);
          } else if (be.IsSequence()) {
            if (be.size() != 2)
              throw InfoError("Beam energies have to be a list of either numbers or pairs of numbers");
            beam_energy_pairs += make_pair(be[0].as<double>(), be[1].as<double>());
          } else {
            throw InfoError("Beam energies have to be a list of either numbers or pairs of numbers");
          }
        }
        ai->_energies = beam_energy_pairs;
      }

      #endif

    } catch (...) { THROW_INFOERR("Energies"); }

    #undef THROW_INFOERR


    MSG_TRACE("AnalysisInfo pointer = " << ai);
    return ai;
  }


  string toString(const AnalysisInfo& ai) {
    stringstream ss;
    ss << ai.name();
    ss << " - " << ai.summary();
    // ss << " - " << ai.beams();
    // ss << " - " << ai.energies();
    ss << " (" << ai.status() << ")";
    return ss.str();
  }


}
