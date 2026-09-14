# - Locate HepPDT library
# Defines:
#
#  HEPPDT_FOUND
#  HEPPDT_VERSION
#  HEPPDT_PDT_INCLUDE_DIR
#  HEPPDT_PDT_LIBRARY
#  HEPPDT_PID_LIBRARY
#  HEPPDT_PID_INCLUDE_DIR

find_path(HEPPDT_PDT_INCLUDE_DIR HepPDT/ParticleData.hh
          HINTS $ENV{HEPPDT_ROOT_DIR}/include ${HEPPDT_ROOT_DIR}/include)
find_path(HEPPDT_PID_INCLUDE_DIR HepPDT/ParticleData.hh
          HINTS $ENV{HEPPDT_ROOT_DIR}/include ${HEPPDT_ROOT_DIR}/include)
find_library(HEPPDT_PDT_LIBRARY NAMES HepPDT
          HINTS $ENV{HEPPDT_ROOT_DIR}/lib $ENV{HEPPDT_ROOT_DIR}/lib64 ${HEPPDT_ROOT_DIR}/lib ${HEPPDT_ROOT_DIR}/lib64)
find_library(HEPPDT_PID_LIBRARY NAMES HepPID
          HINTS $ENV{HEPPDT_ROOT_DIR}/lib $ENV{HEPPDT_ROOT_DIR}/lib64 ${HEPPDT_ROOT_DIR}/lib ${HEPPDT_ROOT_DIR}/lib64)

# --- Version detection ---
set(_heppdt_defs_header "${HEPPDT_PDT_INCLUDE_DIR}/HepPDT/defs.h")
if(EXISTS "${_heppdt_defs_header}")
  file(READ "${_heppdt_defs_header}" _heppdt_defs_content)
  string(REGEX MATCH "#define[ \t]+VERSION[ \t]+\"([0-9]+\\.[0-9]+\\.[0-9]+)\""
         _match "${_heppdt_defs_content}")
  if(_match)
    set(HEPPDT_VERSION "${CMAKE_MATCH_1}")
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HepPDT
  REQUIRED_VARS HEPPDT_PDT_INCLUDE_DIR HEPPDT_PID_INCLUDE_DIR
                HEPPDT_PDT_LIBRARY HEPPDT_PID_LIBRARY
  VERSION_VAR HEPPDT_VERSION
)

if(HEPPDT_FOUND) 
  if (NOT TARGET HepPDT::HepPDT)
    add_library(HepPDT::HepPDT INTERFACE IMPORTED)
    set_target_properties(HepPDT::HepPDT PROPERTIES
        INTERFACE_LINK_LIBRARIES "${HEPPDT_PDT_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${HEPPDT_PDT_INCLUDE_DIR}"
    )
  endif()

  if(NOT TARGET HepPDT::HepPID)
    add_library(HepPDT::HepPID INTERFACE IMPORTED)
    set_target_properties(HepPDT::HepPID PROPERTIES
        INTERFACE_LINK_LIBRARIES "${HEPPDT_PID_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${HEPPDT_PID_INCLUDE_DIR}"
    )
  endif()
endif()

mark_as_advanced(HEPPDT_FOUND HEPPDT_PDT_INCLUDE_DIR HEPPDT_PID_INCLUDE_DIR
                 HEPPDT_PDT_LIBRARY HEPPDT_PID_LIBRARY HEPPDT_VERSION)

