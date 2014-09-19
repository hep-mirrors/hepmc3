include ../make.inc
HEPMC3LOCATION = ..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

TESTS = basic_tree.exe \
        HepMC2_to_3_converter_test.exe \
        hepevt_wrapper_test_main.exe

all: $(TESTS)

# special case: for this example we have to compile Fortran code as well
hepevt_wrapper_test_main.exe: hepevt_wrapper_test_main.o
	gfortran -fno-automatic -fno-backslash -ffixed-line-length-132 -c hepevt_wrapper_test.f
	$(LD) $(LDFLAGS) $< hepevt_wrapper_test.o -o $@ $(HEPMC3LIBS) -lgfortran -Wl,-rpath,$(HEPMC3LOCATION)/lib

%.exe: %.o
	$(LD) $(LDFLAGS) $< -o $@ $(HEPMC3LIBS) -Wl,-rpath,$(HEPMC3LOCATION)/lib

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(HEPMC3INC)

clean:
	rm -f *.o *.exe catch/*.o catch/*.exe
