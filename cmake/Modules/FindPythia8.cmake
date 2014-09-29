# Locate Pythia8 library
# in a directory defined via PYTHIA8_ROOT_DIR environment variable
#
# Defines:
#  PYTHIA8_FOUND
#  PYTHIA8_LIBRARIES

find_path(PYTHIA8_INCLUDE_DIR Pythia8/Pythia.h
          HINTS $ENV{PYTHIA8_ROOT_DIR}/include ${PYTHIA8_ROOT_DIR}/include
          $ENV{PYTHIA8_DIR}/include ${PYTHIA8_DIR}/include)

find_library(PYTHIA8_LIBRARIES NAMES pythia8
             HINTS $ENV{PYTHIA8_ROOT_DIR}/lib ${PYTHIA8_ROOT_DIR}/lib
             HINTS $ENV{PYTHIA8_DIR}/lib ${PYTHIA8_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set PYTHIA8_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pythia8 DEFAULT_MSG PYTHIA8_INCLUDE_DIR PYTHIA8_LIBRARIES)

mark_as_advanced(PYTHIA8_FOUND PYTHIA8_LIBRARIES)
