include ../make.inc
HEPMC3LOCATION = ..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

TESTS      = basic_tree.exe HepMC2_to_3_converter_test.exe

all: $(TESTS)

%.exe: %.o
	$(LD) $(LDFLAGS) $< -o $@ $(HEPMC3LIBS) -Wl,-rpath,$(HEPMC3LOCATION)/lib

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(HEPMC3INC)

clean:
	rm -f *.o *.exe catch/*.o catch/*.exe
