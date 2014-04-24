#ifndef VALIDATION_TOOL_H
#define VALIDATION_TOOL_H

#ifdef HEPMC2

// Ignore HepMC3 code, use HepMC2 code
#define HEPMC2CODE( x ) x
#define HEPMC3CODE( x )
using namespace HepMC;

#else

// Ignore HepMC2 code, use HepMC3 code
#define HEPMC2CODE( x )
#define HEPMC3CODE( x ) x
using namespace HepMC3;

#endif // ifdef HEPMC2

class ValidationTool {
public:
    virtual ~ValidationTool() {};
public:
    virtual const std::string name()                   = 0;
    virtual bool              tool_modifies_event()    = 0;
    virtual void              initialize()             = 0;
    virtual int               process(GenEvent &hepmc) = 0;
    virtual void              finalize()               = 0;
};

#endif
