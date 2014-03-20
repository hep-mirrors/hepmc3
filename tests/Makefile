include ../make.inc
HEPMC3LOCATION = ..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

UNIT_TESTS = catch/main.exe
TESTS      = basic_tree.exe

all: $(TESTS) $(UNIT_TESTS)

%.exe: %.o
	$(LD) $(LDFLAGS) $< -o $@ $(HEPMC3LIBS) -Wl,-rpath,../lib

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(HEPMC3INC)

clean:
	rm -f *.o *.exe catch/*.o catch/*.exe
