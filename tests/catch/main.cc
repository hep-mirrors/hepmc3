#include <iostream>
#include "HepMC3/GenEvent.h"

int main() {
  std::cout<<"Test01"<<std::endl;

  HepMC3::GenEvent *evt = new HepMC3::GenEvent();

  evt->print();

  return 0;
}

