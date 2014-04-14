include make.inc

LIB_VER       = 3.00.00
LIB_HEPMC_A   = libHepMC.a
LIB_HEPMC_SO  = libHepMC.so
LIB_HEPMC_OBJ = src/Log.o src/GenParticle.o src/GenVertex.o src/GenEvent.o \
                src/Search/FilterBase.o src/Search/Filter.o src/Search/FilterList.o src/Search/FindParticles.o \
                src/IO_Base.o src/IO_GenEvent.o src/IO_HepMC2_adapter.o \

all: lib

lib: $(LIB_HEPMC_OBJ)
	mkdir -p lib
	ar cr lib/$(LIB_HEPMC_A) $(LIB_HEPMC_OBJ)
	$(LD) $(LDFLAGS) $(SOFLAGS) -o lib/$(LIB_HEPMC_SO).$(LIB_VER) $(LIB_HEPMC_OBJ)
	ln -sf $(LIB_HEPMC_SO).$(LIB_VER) lib/$(LIB_HEPMC_SO)

%.o: %.cc
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -f src/*.o src/Search/*.o lib/*
