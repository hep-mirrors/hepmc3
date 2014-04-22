include make.inc
include paths.inc

HEPMC3LOCATION = ../..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

#
# Libraries to build
#
LIB_PHOTOS_HEPMC3_INT_SO  = libPhotosHepMC3Interface.so
LIB_PHOTOS_HEPMC3_INT_OBJ = $(PHOTOSINTERFACE)/src/PhotosHepMC3Event.o \
                            $(PHOTOSINTERFACE)/src/PhotosHepMC3Particle.o

LIB_TAUOLA_HEPMC3_INT_SO  = libTauolaHepMC3Interface.so
LIB_TAUOLA_HEPMC3_INT_OBJ = $(TAUOLAINTERFACE)/src/TauolaHepMC3Event.o \
                            $(TAUOLAINTERFACE)/src/TauolaHepMC3Particle.o

LIB_MCTESTER_HEPMC3_INT_SO  = libHepMC3Interface.so
LIB_MCTESTER_HEPMC3_INT_OBJ = $(MCTESTERINTERFACE)/src/HepMC3Event.o \
                              $(MCTESTERINTERFACE)/src/HepMC3Particle.o

LIB_PYTHIA8_HEPMC3_INT_SO  = libpythia8tohepmc3.so
LIB_PYTHIA8_HEPMC3_INT_OBJ = $(PYTHIA8INTERFACE)/src/Pythia8ToHepMC3.o

LIBS     =
TESTS    =
INCLUDES = $(HEPMC3INC)
LINKS    = $(HEPMC3LIBS)
RPATHS   = -Wl,-rpath,$(HEPMC3LOCATION)/lib

#
# Standalone tests
#
ifneq ($(MCTESTERLOCATION), )
  override LIBS     += $(LIB_MCTESTER_HEPMC3_INT_SO)
  override INCLUDES += $(MCTESTERINC) $(ROOTINC)
  override LINKS    += $(MCTESTERLIBS) $(ROOTLIBS)
  override RPATHS   += -Wl,-rpath,$(MCTESTERLOCATION)/lib -Wl,-rpath,$(MCTESTERINTERFACE)/lib -Wl,-rpath,$(shell root-config --libdir)
endif

ifneq ($(PHOTOSLOCATION), )
  override LIBS     += $(LIB_PHOTOS_HEPMC3_INT_SO)
  override INCLUDES += $(PHOTOSINC)
  override LINKS    += $(PHOTOSLIBS)
  override RPATHS   += -Wl,-rpath,$(PHOTOSLOCATION)/lib -Wl,-rpath,$(PHOTOSINTERFACE)/lib
  override TESTS    += photos_standalone.exe
endif

ifneq ($(TAUOLALOCATION), )
  override LIBS     += $(LIB_TAUOLA_HEPMC3_INT_SO)
  override INCLUDES += $(TAUOLAINC)
  override LINKS    += $(TAUOLALIBS)
  override RPATHS   += -Wl,-rpath,$(TAUOLALOCATION)/lib -Wl,-rpath,$(TAUOLAINTERFACE)/lib
  override TESTS    += tauola_standalone.exe
endif

ifneq ($(PYTHIA8LOCATION), )
  override LIBS     += $(LIB_PYTHIA8_HEPMC3_INT_SO)
  override INCLUDES += $(PYTHIA8INC)
  override LINKS    += $(PYTHIA8LIBS)
  override RPATHS   += -Wl,-rpath,$(PYTHIA8LOCATION)/lib -Wl,-rpath,$(PYTHIA8INTERFACE)/lib
  override TESTS    += pythia8_standalone.exe
endif

#
# Combined tests
#
ifneq ($(PHOTOSLOCATION), )
ifneq ($(MCTESTERLOCATION), )
  override TESTS    += photos_mctester.exe
endif
endif

ifneq ($(TAUOLALOCATION), )
ifneq ($(MCTESTERLOCATION), )
  override TESTS    += tauola_mctester.exe
endif
endif

ifneq ($(TAUOLALOCATION), )
ifneq ($(PHOTOSLOCATION), )

  override TESTS    += tauola_photos.exe
ifneq ($(MCTESTERLOCATION), )
  override TESTS    += tauola_photos_mctester.exe
endif

endif
endif

all: $(LIBS) $(TESTS)

# Build Photos library
$(LIB_PHOTOS_HEPMC3_INT_SO): $(LIB_PHOTOS_HEPMC3_INT_OBJ)
	mkdir -p $(PHOTOSINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PHOTOSINTERFACE)/lib/$@ $^

# Build Tauola library
$(LIB_TAUOLA_HEPMC3_INT_SO): $(LIB_TAUOLA_HEPMC3_INT_OBJ)
	mkdir -p $(TAUOLAINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(TAUOLAINTERFACE)/lib/$@ $^

# Build pythia8 library
$(LIB_PYTHIA8_HEPMC3_INT_SO): $(LIB_PYTHIA8_HEPMC3_INT_OBJ)
	mkdir -p $(PYTHIA8INTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PYTHIA8INTERFACE)/lib/$@ $^

# Build MC-TESTER library
$(LIB_MCTESTER_HEPMC3_INT_SO): $(LIB_MCTESTER_HEPMC3_INT_OBJ)
	mkdir -p $(MCTESTERINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(MCTESTERINTERFACE)/lib/$@ $^

# Build programs
%.exe: %.o
	$(LD) $(LDFLAGS) $< -o $@ $(LINKS) $(RPATHS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cxx
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f *.o *.exe

Clean: clean
	rm -f */src/*.o */lib/*.so
