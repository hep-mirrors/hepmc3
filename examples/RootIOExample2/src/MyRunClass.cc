#include "MyRunClass.h"

MyRunClass::MyRunClass():run(0){};

void MyRunClass::SetRunInfo(HepMC::GenRunInfo* myrun)
{
  run = myrun;
}

HepMC::GenRunInfo* MyRunClass::GetRunInfo()
{
  return run;
}
