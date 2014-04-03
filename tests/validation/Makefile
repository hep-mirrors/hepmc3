include make.inc
include paths.inc

HEPMC3LOCATION = ../..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

LIB_PHOTOS_HEPMC3_INT_SO  = libPhotosHepMC3Interface.so
LIB_PHOTOS_HEPMC3_INT_OBJ = $(PHOTOSINTERFACE)/src/PhotosHepMC3Event.o \
                            $(PHOTOSINTERFACE)/src/PhotosHepMC3Particle.o

LIB_MCTESTER_HEPMC3_INT_SO  = libHepMC3Interface.so
LIB_MCTESTER_HEPMC3_INT_OBJ = $(MCTESTERINTERFACE)/src/HepMC3Event.o \
                              $(MCTESTERINTERFACE)/src/HepMC3Particle.o

LIBS     =
TESTS    =
INCLUDES = $(HEPMC3INC)
LINKS    = $(HEPMC3LIBS)
RPATHS   = -Wl,-rpath,$(HEPMC3LOCATION)/lib

ifneq ($(MCTESTERLOCATION), )
  override LIBS     += $(LIB_MCTESTER_HEPMC3_INT_SO)
  override INCLUDES += $(MCTESTERINC)
  override LINKS    += $(MCTESTERLIBS)
  override RPATHS   += -Wl,-rpath,$(MCTESTERLOCATION)/lib -Wl,-rpath,$(MCTESTERINTERFACE)/lib
endif

ifneq ($(PHOTOSLOCATION), )

  override LIBS     += $(LIB_PHOTOS_HEPMC3_INT_SO)
  override INCLUDES += $(PHOTOSINC)
  override LINKS    += $(PHOTOSLIBS)
  override RPATHS   += -Wl,-rpath,$(PHOTOSLOCATION)/lib -Wl,-rpath,$(PHOTOSINTERFACE)/lib
  override TESTS    += photos_standalone.exe
ifneq ($(MCTESTERLOCATION), )
  override TESTS    += photos_mctester.exe
endif
endif

all: $(LIBS) $(TESTS)

# Build Photos library
$(LIB_PHOTOS_HEPMC3_INT_SO): $(LIB_PHOTOS_HEPMC3_INT_OBJ)
	mkdir -p $(PHOTOSINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PHOTOSINTERFACE)/lib/$@ $^

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
	rm -f *.o *.exe */src/*.o */lib/*.so
