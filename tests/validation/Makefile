include make.inc
include paths.inc

HEPMC3LOCATION = ../..
HEPMC3INC      = -I$(HEPMC3LOCATION)/include
HEPMC3LIBS     = -L$(HEPMC3LOCATION)/lib -lHepMC

LIB_PHOTOS_HEPMC3_INT_SO  = libPhotosHepMC3Interface.so
LIB_PHOTOS_HEPMC3_INT_OBJ = $(PHOTOSINTERFACE)/src/PhotosHepMC3Event.o \
                            $(PHOTOSINTERFACE)/src/PhotosHepMC3Particle.o

LIBS  = $(LIB_PHOTOS_HEPMC3_INT_SO)
TESTS = photos_standalone.exe

all: $(LIBS) $(TESTS)

# Build Photos library
$(LIB_PHOTOS_HEPMC3_INT_SO): $(LIB_PHOTOS_HEPMC3_INT_OBJ)
	mkdir -p $(PHOTOSINTERFACE)/lib/
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $(PHOTOSINTERFACE)/lib/$(LIB_PHOTOS_HEPMC3_INT_SO) $(LIB_PHOTOS_HEPMC3_INT_OBJ)

# Build Photos example program
photos_standalone.exe: photos_standalone.o
	$(LD) $(LDFLAGS) $< -o $@ $(HEPMC3LIBS) $(PHOTOSLIBS) \
	-Wl,-rpath,$(HEPMC3LOCATION)/lib \
	-Wl,-rpath,$(PHOTOSLOCATION)/lib \
	-Wl,-rpath,$(PHOTOSINTERFACE)/lib

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(HEPMC3INC) $(PHOTOSINC)

clean:
	rm -f *.o *.exe */src/*.o */lib/*.so
