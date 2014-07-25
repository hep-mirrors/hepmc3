#include "Rivet/AnalysisHandler.hh"
#include "Rivet/AnalysisLoader.hh"
#include "HepMC3/IO/IO_GenEvent.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <hepmcfile> <ana1> [<ana2> ...]" << endl;
    return 1;
  }

  foreach (const string& a, Rivet::AnalysisLoader::analysisNames())
    cout << a << endl;

  Rivet::AnalysisHandler ah;
  for (int i = 2; i < argc; ++i) {
    ah.addAnalysis(argv[i]);
  }

  HepMC3::IO_GenEvent hepmcio(argv[1], std::ios::in);
  HepMC3::GenEvent* evt = new HepMC3::GenEvent(HepMC3::Units::GEV,HepMC3::Units::MM);
  while(!hepmcio.rdstate()) {
    hepmcio.fill_next_event(*evt);
    if( hepmcio.rdstate() ) break;
    ah.analyze(*evt);
  }
  delete evt;

  ah.setCrossSection(1.0);
  ah.finalize();
  ah.writeData("Rivet.yoda");

  return 0;
}
