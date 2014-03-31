include ../../make.inc
HEPMC3LOCATION = ../..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

MAIN_PROGRAM = catch.exe
MAIN_OBJECT  = catch.o
.PRECIOUS: $(MAIN_OBJECT)

UNIT_TESTS   = GenParticle_unit_tests.o

all: $(MAIN_PROGRAM)

%.exe: $(MAIN_OBJECT) $(UNIT_TESTS)
	$(LD) $(LDFLAGS) $^ -o $@ $(HEPMC3LIBS) -Wl,-rpath,$(HEPMC3LOCATION)/lib

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(HEPMC3INC)

clean:
	rm -f *.o *.exe
