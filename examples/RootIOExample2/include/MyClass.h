#include "HepMC/GenEvent.h"

class MyClass
{

 public:

  MyClass();
  void SetEvent(HepMC::GenEvent*);
  HepMC::GenEvent* GetEvent();

  
 private:
  int someint;
  HepMC::GenEvent* event;
  
};
