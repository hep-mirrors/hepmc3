#include "MyClass.h"

MyClass::MyClass():event(0){};

void MyClass::SetEvent(HepMC::GenEvent* myevt)
{
  event = myevt;
}

HepMC::GenEvent* MyClass::GetEvent()
{
  return event;
}
