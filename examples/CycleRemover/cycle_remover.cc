#include "CycleRemover.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderFactory.h"
#include "WriterDOTCycleRemover.h"
#include <iostream>
#include <memory>
#include <unordered_set>

using namespace HepMC3;

int main(int argc, char **argv) {
  if (argc < 2)
    return 1;
  std::shared_ptr<Reader> input_file = deduce_reader(argv[1]);
  if (!input_file)
    return 2;
  while (!input_file->failed()) {
    GenEvent evt(Units::GEV, Units::MM);
    bool res_read = input_file->read_event(evt);

    if (input_file->failed()) {
      printf("End of file reached. Exit.\n");
      break;
    }
    // Assign "original" attribute to each particle
    for (auto &particle : evt.particles()) {
      particle->add_attribute("original", std::make_shared<HepMC3::IntAttribute>(particle->id()));
    }
    WriterDOTCycleRemover(std::string("before_") + std::to_string(evt.event_number()) + ".dot").write_event(evt);
    auto new_event0 = CycleRemover(evt).merge_cycle(0);
    WriterDOTCycleRemover(std::string("after0_") + std::to_string(evt.event_number()) + ".dot").write_event(*new_event0.get());
    auto new_event1 = CycleRemover(evt).merge_cycle(1);
    WriterDOTCycleRemover(std::string("after1_") + std::to_string(evt.event_number()) + ".dot").write_event(*new_event1.get());
  }
  if (input_file)
    input_file->close();
  return 0;
}
