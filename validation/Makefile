include make.inc

.DEFAULT_GOAL=all

INTERFACES    =
INCLUDES      = -Iinclude
RPATHS_HEPMC3 =
LINKS_HEPMC3  =
DEFS_HEPMC3   =

RPATHS_HEPMC2 =
LINKS_HEPMC2  =
DEFS_HEPMC2   =

# Paths to the tools
# Change paths in this file to include/exclude selected tools
include make.toolslocation.inc

override INCLUDES_HEPMC3 += -I$(HEPMC3LOCATION)/include
override RPATHS_HEPMC3   += -Wl,-rpath,$(HEPMC3LOCATION)/lib
override LINKS_HEPMC3    += -L$(HEPMC3LOCATION)/lib -lHepMC

ifneq ($(HEPMC2LOCATION), )
  override INCLUDES_HEPMC2 += -I$(HEPMC2LOCATION)/include
  override RPATHS_HEPMC2   += -Wl,-rpath,$(HEPMC2LOCATION)/lib
  override LINKS_HEPMC2    += -L$(HEPMC2LOCATION)/lib -lHepMC
  override DEFS_HEPMC2     += -DHEPMC2
endif

VALIDATION_SRC= validation.cc \
                src/ValidationControl.cc \
                src/SimpleEventTool.cc \
                src/FileValidationTool.cc

# Definitions for compiling interfaces to the tools
# Uses paths set by make.toolslocation.inc
# Updates INTERFACES, INCLUDES, VALIDATION_SRC
#         LINKS_HEPMC3, RPATHS_HEPMC3, DEFS_HEPMC3,
#         LINKS_HEPMC2, RPATHS_HEPMC2, DEFS_HEPMC2
include make.tools.inc

all:
ifneq ($(HEPMC3LOCATION), )
ifneq ($(INTERFACES), )
	+@make $(INTERFACES)
endif
	+@make validation.exe
endif
ifneq ($(HEPMC2LOCATION), )
	+@make validation_HEPMC2.exe
endif

# Definitions for HepMC2 files
validation_HEPMC2.exe: $(VALIDATION_SRC:.cc=.o_hepmc2)
	$(LD) $(LDFLAGS) $^ -o $@ $(LINKS_HEPMC2) $(RPATHS_HEPMC2)

%.o_hepmc2: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(DEFS_HEPMC2) $(INCLUDES) $(INCLUDES_HEPMC2)

# Definitions for HepMC files
validation.exe: $(VALIDATION_SRC:.cc=.o)
	$(LD) $(LDFLAGS) $^ -o $@ $(LINKS_HEPMC3) $(RPATHS_HEPMC3)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(INCLUDES) $(INCLUDES_HEPMC3)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DEFS_HEPMC3) $(INCLUDES) $(INCLUDES_HEPMC3)

clean:
	rm -f src/*.o *.o src/*.o_hepmc2 *.o_hepmc2 *.exe

Clean: clean
	rm -f $(INTERFACEDIRECTORY)*/src/*.o  $(INTERFACEDIRECTORY)*/lib/*.so
