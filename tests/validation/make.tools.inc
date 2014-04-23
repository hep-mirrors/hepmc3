INTERFACEDIRECTORY = .

#
# Photos++
#
PHOTOSINTERFACE = $(INTERFACEDIRECTORY)/Photospp

ifneq ($(PHOTOSLOCATION), )
  override LIBS         += libPhotosHepMC3Interface.so
  override INCLUDES     += -I$(PHOTOSLOCATION)/include \
                           -I$(PHOTOSINTERFACE)/include
  override LINKS        += -L$(PHOTOSLOCATION)/lib  -lPhotosCxxInterface -lPhotosFortran
  override LINKS_HEPMC3 += -L$(PHOTOSINTERFACE)/lib -lPhotosHepMC3Interface
  override RPATHS       += -Wl,-rpath,$(PHOTOSLOCATION)/lib \
                           -Wl,-rpath,$(PHOTOSINTERFACE)/lib
  override CFLASG       += -DPHOTOSPP
endif

libPhotosHepMC3Interface.so: $(PHOTOSINTERFACE)/src/PhotosHepMC3Event.o \
                             $(PHOTOSINTERFACE)/src/PhotosHepMC3Particle.o
	mkdir -p $(PHOTOSINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PHOTOSINTERFACE)/lib/$@ $^

#
# Tauola++
#
TAUOLAINTERFACE = $(INTERFACEDIRECTORY)/Tauolapp

ifneq ($(TAUOLALOCATION), )
  override LIBS         += libTauolaHepMC3Interface.so
  override INCLUDES     += -I$(TAUOLALOCATION)/include \
                           -I$(TAUOLAINTERFACE)/include
  override LINKS        += -L$(TAUOLALOCATION)/lib  -lTauolaCxxInterface -lTauolaFortran
  override LINKS_HEPMC3 += -L$(TAUOLAINTERFACE)/lib -lTauolaHepMC3Interface
  override RPATHS       += -Wl,-rpath,$(TAUOLALOCATION)/lib \
                           -Wl,-rpath,$(TAUOLAINTERFACE)/lib
  override CFLASG       += -DTAUOLAPP
endif

libTauolaHepMC3Interface.so: $(TAUOLAINTERFACE)/src/TauolaHepMC3Event.o \
                             $(TAUOLAINTERFACE)/src/TauolaHepMC3Particle.o
	mkdir -p $(TAUOLAINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(TAUOLAINTERFACE)/lib/$@ $^

#
# pythia8
#
PYTHIA8INTERFACE = $(INTERFACEDIRECTORY)/pythia8

ifneq ($(PYTHIA8LOCATION), )
  override LIBS         += libpythia8tohepmc3.so
  override INCLUDES     += -I$(PYTHIA8LOCATION)/include \
                           -I$(PYTHIA8INTERFACE)/include
  override LINKS        += -L$(PYTHIA8LOCATION)/lib   -lpythia8 -llhapdfdummy \
                           -L$(PYTHIA8INTERFACE)/lib -lpythia8tohepmc
  override LINKS_HEPMC3 += -L$(PYTHIA8LOCATION)/lib   -lpythia8 -llhapdfdummy \
                           -L$(PYTHIA8INTERFACE)/lib -lpythia8tohepmc3
  override RPATHS       += -Wl,-rpath,$(PYTHIA8LOCATION)/lib \
                           -Wl,-rpath,$(PYTHIA8INTERFACE)/lib
  override CFLASG       += -DPYTHIA8
endif

libpythia8tohepmc3.so: $(PYTHIA8INTERFACE)/src/Pythia8ToHepMC3.o
	mkdir -p $(PYTHIA8INTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PYTHIA8INTERFACE)/lib/$@ $^

#
# MC-TESTER
#
MCTESTERINTERFACE = $(INTERFACEDIRECTORY)/mc-tester

ifneq ($(MCTESTERLOCATION), )
  override LIBS         += libHepMC3Interface.so
  override INCLUDES     += -I$(MCTESTERLOCATION)/include \
                           -I$(MCTESTERINTERFACE)/include \
                           -I$(shell root-config --incdir)
  override LINKS        += -L$(MCTESTERLOCATION)/lib  -lHEPEvent -lMCTester \
                           $(shell root-config --glibs)
  override LINKS_HEPMC3 += -L$(MCTESTERINTERFACE)/lib -lHepMC3Interface
  override RPATHS       += -Wl,-rpath,$(MCTESTERLOCATION)/lib \
                           -Wl,-rpath,$(MCTESTERINTERFACE)/lib \
                           -Wl,-rpath,$(shell root-config --libdir)
  override CFLASG       += -MCTESTER
endif

libHepMC3Interface.so: $(MCTESTERINTERFACE)/src/HepMC3Event.o \
                       $(MCTESTERINTERFACE)/src/HepMC3Particle.o
	mkdir -p $(MCTESTERINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(MCTESTERINTERFACE)/lib/$@ $^
