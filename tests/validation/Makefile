include make.inc

# Define HEPMCLOCATION, HEPMCINC, HEPMCLIBS
include make.hepmclocation.inc

LIBS     =
TESTS    =
INCLUDES = -I$(HEPMCLOCATION)/include -Itools
LINKS    = -L$(HEPMCLOCATION)/lib -lHepMC
RPATHS   = -Wl,-rpath,$(HEPMCLOCATION)/lib

.DEFAULT_GOAL=all

# Paths to the tools
# Change paths in this file to include/exclude selected tools
include make.toolslocation.inc

# Definitions for compiling interfaces to the tools
# Uses:    paths set by all of the above .inc files
# Changes: LIBS, TESTS, INCLUDES, LINKS, RPATHS
include make.tools.inc

all: $(LIBS) validation.exe

validation.exe: tools/ValidationControl.o validation.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LINKS) $(LINKS_HEPMC3) $(RPATHS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cxx
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f *.o *.exe

Clean: clean
	rm -f $(INTERFACEDIRECTORY)*/src/*.o  $(INTERFACEDIRECTORY)*/lib/*.so
