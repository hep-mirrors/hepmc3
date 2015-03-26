#include "HepMC/LHEFAttributes.h"
#include "HepMC/ReaderAscii.h"
#include "HepMC/WriterAscii.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include <iomanip>

using namespace HepMC;
using namespace LHEF;

int main(int argc, char ** argv) {

  // Create Reader and Writer object
  LHEF::Reader reader("LHEF_example.lhe");

  shared_ptr<HEPRUPAttribute> hepr = make_shared<HEPRUPAttribute>();

  hepr->heprup = reader.heprup;

  hepr->tags = XMLTag::findXMLTags(reader.headerBlock + reader.initComments);

  shared_ptr<GenRunInfo> runinfo = make_shared<GenRunInfo>();

  runinfo->add_attribute("HEPRUP", hepr);

  vector<string> weightnames;
  for ( int i = 0, N = hepr->heprup.weightinfo.size(); i < N; ++i )
    weightnames.push_back(hepr->heprup.weightNameHepMC(i));
  runinfo->set_weight_names(weightnames);


  WriterAscii output("LHEF_example.hepmc3", runinfo);

  int neve = 0;

  while ( reader.readEvent() ) {
    ++neve;

    shared_ptr<HEPEUPAttribute> hepe = make_shared<HEPEUPAttribute>();

    if ( reader.outsideBlock.length() )
      hepe->tags =  XMLTag::findXMLTags(reader.outsideBlock);
    hepe->hepeup = reader.hepeup;

    GenEvent ev(runinfo, Units::GEV, Units::MM);
    ev.set_event_number(neve);
    ev.add_attribute("HEPEUP", hepe);


    GenParticlePtr p1 = make_shared<GenParticle>(hepe->momentum(0),
						 hepe->hepeup.IDUP[0],
						 hepe->hepeup.ISTUP[0]);
    GenParticlePtr p2 = make_shared<GenParticle>(hepe->momentum(1),
						 hepe->hepeup.IDUP[1],
						 hepe->hepeup.ISTUP[1]);

    GenVertexPtr vx = make_shared<GenVertex>();
    vx->add_particle_in(p1);
    vx->add_particle_in(p2);

    for ( int i = 2; i < hepe->hepeup.NUP; ++i )
      vx->add_particle_out(make_shared<GenParticle>(hepe->momentum(i),
						  hepe->hepeup.IDUP[i],
						  hepe->hepeup.ISTUP[i]));
    ev.add_vertex(vx);

    output.write_event(ev);

  }

  output.close();

  ReaderAscii input("LHEF_example.hepmc3");
  LHEF::Writer writer("LHEF_example_out.lhe");
  hepr = shared_ptr<HEPRUPAttribute>();

  while ( true ) {

    GenEvent ev(Units::GEV, Units::MM);

    if ( !input.read_event(ev) || ev.event_number() == 0 ) break;

    if ( !hepr ) {
      hepr = ev.attribute<HEPRUPAttribute>("HEPRUP");
      for ( int i = 0, N = hepr->tags.size(); i < N; ++i )
	if ( hepr->tags[i]->name != "init" )
	  hepr->tags[i]->print(writer.headerBlock());
      writer.heprup = hepr->heprup;
      writer.init();
    }

    shared_ptr<HEPEUPAttribute> hepe = ev.attribute<HEPEUPAttribute>("HEPEUP");
    for ( int i = 0, N = hepe->tags.size(); i < N; ++i )
	if ( hepe->tags[i]->name != "event" &&
	     hepe->tags[i]->name != "eventgroup" )
	  hepe->tags[i]->print(writer.eventComments());
      writer.hepeup = hepe->hepeup;
      writer.hepeup.heprup =  &writer.heprup;
      writer.writeEvent();

  }

}
