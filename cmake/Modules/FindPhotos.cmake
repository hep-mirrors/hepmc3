# - Locate Photospp library
# Defines:
#
#  PHOTOS_FOUND
#  PHOTOS_VERSION
#  PHOTOS_INCLUDE_DIR
#  PHOTOS_INCLUDE_DIRS (not cached)
#  PHOTOS_LIBRARY
#  PHOTOS_LIBRARIES (not cached) : includes libraries

find_path(PHOTOS_INCLUDE_DIR Photos.h
  HINTS $ENV{PHOTOS_ROOT_DIR}/include ${PHOTOS_ROOT_DIR}/include)

if(PHOTOS_INCLUDE_DIR)
  find_library(PHOTOS_LIBRARY NAMES PhotosCxxInterface
    HINTS $ENV{PHOTOS_ROOT_DIR}/lib ${PHOTOS_ROOT_DIR}/lib)

  set(PHOTOS_INCLUDE_DIRS ${PHOTOS_INCLUDE_DIR})
  set(PHOTOS_LIBRARIES ${PHOTOS_LIBRARY})
endif()

# handle the QUIETLY and REQUIRED arguments and set PHOTOS_FOUND to TRUE if
# all listed variables are TRUE

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Photos DEFAULT_MSG PHOTOS_INCLUDE_DIR PHOTOS_LIBRARY)

mark_as_advanced(PHOTOS_FOUND PHOTOS_INCLUDE_DIR PHOTOS_LIBRARY)

