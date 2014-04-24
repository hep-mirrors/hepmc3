include make.inc

.DEFAULT_GOAL=all

INTERFACES    =
INCLUDES      =
RPATHS_HEPMC3 =
LINKS_HEPMC3  =
DEFS_HEPMC3   =

RPATHS_HEPMC2 =
LINKS_HEPMC2  =
DEFS_HEPMC2   =

# Paths to the tools
# Change paths in this file to include/exclude selected tools
include make.toolslocation.inc

override INCLUDES      += -I$(HEPMC3LOCATION)/include -Itools
override RPATHS_HEPMC3 += -Wl,-rpath,$(HEPMC3LOCATION)/lib
override LINKS_HEPMC3  += -L$(HEPMC3LOCATION)/lib -lHepMC

ifneq ($(HEPMC2LOCATION), )
  override INCLUDES      += -I$(HEPMC2LOCATION)/include
  override RPATHS_HEPMC2 += -Wl,-rpath,$(HEPMC2LOCATION)/lib
  override LINKS_HEPMC2  += -L$(HEPMC2LOCATION)/lib -lHepMC
  override DEFS_HEPMC2   += -DHEPMC2
endif

# Definitions for compiling interfaces to the tools
# Uses paths set by make.toolslocation.inc
# Updates INTERFACES, INCLUDES, LINKS_HEPMC3, RPATHS_HEPMC3, DEFS_HEPMC3,
#                               LINKS_HEPMC2, RPATHS_HEPMC2, DEFS_HEPMC2
include make.tools.inc

all:
ifneq ($(HEPMC3LOCATION), )
	@make $(INTERFACES)
	@make validation.exe
endif
ifneq ($(HEPMC2LOCATION), )
	@make validation_HEPMC2.exe
endif

# Definitions for HepMC2 files
validation_HEPMC2.exe: tools/ValidationControl_HEPMC2.o validation_HEPMC2.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LINKS_HEPMC2) $(RPATHS_HEPMC2)

validation_HEPMC2.o: validation.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(DEFS_HEPMC2) $(INCLUDES)

tools/ValidationControl_HEPMC2.o: tools/ValidationControl.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(DEFS_HEPMC2) $(INCLUDES)

# Definitions for HepMC3 files
validation.exe: tools/ValidationControl.o validation.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LINKS_HEPMC3) $(RPATHS_HEPMC3)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(INCLUDES)

%.o: %.cxx
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(INCLUDES)

clean:
	rm -f tools/*.o *.o *.exe

Clean: clean
	rm -f $(INTERFACEDIRECTORY)*/src/*.o  $(INTERFACEDIRECTORY)*/lib/*.so
