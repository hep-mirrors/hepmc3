# - Locate Graphviz library
# in a directory defined via  Graphviz_DIR or Graphviz_DIR.
# Defines:
#
#  Graphviz_FOUND
#  Graphviz_INCLUDE_DIR
#  Graphviz_INCLUDE_DIRS (not cached)
#  Graphviz_CGRAPH_LIBRARY
#  Graphviz_GVC_LIBRARY
if (Graphviz_DIR OR GRAPHVIZ_DIR OR (DEFINED ENV{Graphviz_DIR}) OR (DEFINED ENV{GRAPHVIZ_DIR}) )
  set(Graphviz_SEARCH_DIRS "" CACHE STRING "" FORCE)
  if (Graphviz_DIR)
    list (APPEND Graphviz_SEARCH_DIRS "${Graphviz_DIR}" )
  endif()
  if (GRAPHVIZ_DIR)
    list (APPEND Graphviz_SEARCH_DIRS "${GRAPHVIZ_DIR}" )
  endif()
  if (DEFINED ENV{Graphviz_DIR})
    list (APPEND Graphviz_SEARCH_DIRS "$ENV{Graphviz_DIR}" )
  endif()
  if (DEFINED ENV{GRAPHVIZ_DIR})
    list (APPEND Graphviz_SEARCH_DIRS "ENV{GRAPHVIZ_DIR}" )
  endif()
endif()

if (Graphviz_SEARCH_DIRS)
  find_path(Graphviz_INCLUDE_DIR graphviz/gvc.h PATHS ${Graphviz_SEARCH_DIRS} PATH_SUFFIXES include NO_DEFAULT_PATH)
  find_library(Graphviz_CGRAPH_LIBRARY NAMES cgraph PATHS ${Graphviz_SEARCH_DIRS}  PATH_SUFFIXES lib lib64 NO_DEFAULT_PATH)
  find_library(Graphviz_GVC_LIBRARY NAMES gvc PATHS ${Graphviz_SEARCH_DIRS}  PATH_SUFFIXES lib lib64 NO_DEFAULT_PATH)
else()
  find_path(Graphviz_INCLUDE_DIR graphviz/gvc.h PATH_SUFFIXES include)
  find_library(Graphviz_CGRAPH_LIBRARY NAMES cgraph PATHS_SUFFIXES lib lib64)
  find_library(Graphviz_GVC_LIBRARY NAMES gvc PATHS_SUFFIXES lib lib64)
endif()

set(Graphviz_INCLUDE_DIRS ${Graphviz_INCLUDE_DIR} ${Graphviz_INCLUDE_DIR}/graphviz)
get_filename_component(Graphviz_LIBRARY_DIR ${Graphviz_GVC_LIBRARY} PATH)
set ( TEST_SOURCE "#include <graphviz/gvc.h>\n#include <string>\n #include <graphviz/cdt.h>\n int main(){\nreturn strcmp(\"XX\",\"XXY\");\n}\n")
if (Graphviz_INCLUDE_DIR)
  set(CMAKE_REQUIRED_INCLUDES ${Graphviz_INCLUDE_DIRS})
endif()
check_cxx_source_compiles("${TEST_SOURCE}" TEST_SOURCE_NOAST_COMPILES )
check_cxx_source_compiles("#define _PACKAGE_ast 1\n${TEST_SOURCE}" TEST_SOURCE_AST_COMPILES )
if (TEST_SOURCE_AST_COMPILES AND (NOT TEST_SOURCE_NOAST_COMPILES))
  set(Graphviz_DEFINES "_PACKAGE_ast=1")
else()
  set(Graphviz_DEFINES "_UNUSED_DUMMY_DEFINE")  
endif()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Graphviz FOUND_VAR Graphviz_FOUND REQUIRED_VARS Graphviz_DEFINES Graphviz_INCLUDE_DIR Graphviz_CGRAPH_LIBRARY Graphviz_GVC_LIBRARY  HANDLE_COMPONENTS)


if(Graphviz_FOUND AND NOT TARGET Graphviz::CGRAPH)
    add_library(Graphviz::CGRAPH UNKNOWN IMPORTED)
    set_target_properties(Graphviz::CGRAPH PROPERTIES
        IMPORTED_LOCATION "${Graphviz_CGRAPH_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Graphviz_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS "${Graphviz_DEFINES}"        
    )
endif()
if(Graphviz_FOUND AND NOT TARGET Graphviz::GVC)
    add_library(Graphviz::GVC UNKNOWN IMPORTED)
    set_target_properties(Graphviz::GVC PROPERTIES
        IMPORTED_LOCATION "${Graphviz_GVC_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Graphviz_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS "${Graphviz_DEFINES}"
    )
endif()

mark_as_advanced(Graphviz_FOUND Graphviz_INCLUDE_DIRS Graphviz_CGRAPH_LIBRARY Graphviz_GVC_LIBRARY Graphviz_DEFINES)
