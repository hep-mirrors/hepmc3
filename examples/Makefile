include ../make.inc

# For now, this path must be set manually
PYTHIA8LOCATION      = ../../pythia8185/

HEPMC3LOCATION       = ..
PYTHIA8INTERFACE     = ../tests/validation/interfaces/pythia8
PYTHIA8INTERFACE_OBJ = $(PYTHIA8INTERFACE)/src/Pythia8ToHepMC3.o

INCLUDES = -I$(HEPMC3LOCATION)/include \
           -I$(PYTHIA8LOCATION)/include \
           -I$(PYTHIA8INTERFACE)/include
LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC \
           -L$(PYTHIA8LOCATION)/lib -lpythia8 -llhapdfdummy $(PYTHIA8INTERFACE_OBJ)
RPATHS   = -Wl,-rpath,$(HEPMC3LOCATION)/lib \
           -Wl,-rpath,$(PYTHIA8LOCATION)/lib

all: pythia8_example.exe

pythia8_example.exe: pythia8_example.o $(PYTHIA8INTERFACE_OBJ)
	$(LD) $(LDFLAGS) $< -o $@ $(LIBS) $(RPATHS)

%.exe: %.o
	$(LD) $(LDFLAGS) $< -o $@ $(LIBS) $(RPATHS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f *.o *.exe $(PYTHIA8INTERFACE_OBJ)
