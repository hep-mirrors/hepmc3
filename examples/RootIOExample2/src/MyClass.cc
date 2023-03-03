#include "MyClass.h"

MyClass::MyClass():someint(0), event(nullptr) {}

void MyClass::SetEvent(GenEvent* myevt)
{
    event = myevt;
}

GenEvent* MyClass::GetEvent()
{
    return event;
}

void MyClass::SetInt(int theint)
{
    someint = theint;
}

int MyClass::GetInt() const
{
    return someint;
}
