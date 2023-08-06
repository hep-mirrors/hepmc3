# - Locate Graphviz library
# in a directory defined via  Graphviz_DIR or Graphviz_DIR.
# Defines:
#
#  Graphviz_FOUND
#  Graphviz_INCLUDE_DIR
#  Graphviz_INCLUDE_DIRS (not cached)
#  Graphviz_CGRAPH_LIBRARY
#  Graphviz_GVC_LIBRARY
if (Graphviz_DIR OR Graphviz_DIR OR (DEFINED ENV{Graphviz_DIR}) OR (DEFINED ENV{Graphviz_DIR}) )
  set(Graphviz_SEARCH_DIRS "" CACHE STRING "" FORCE)
  if (Graphviz_DIR)
    list (APPEND Graphviz_SEARCH_DIRS "${Graphviz_DIR}" )
  endif()
  if (Graphviz_DIR)
    list (APPEND Graphviz_SEARCH_DIRS "${Graphviz_DIR}" )
  endif()
  if (DEFINED ENV{Graphviz_DIR})
    list (APPEND Graphviz_SEARCH_DIRS "$ENV{Graphviz_DIR}" )
  endif()
  if (DEFINED ENV{Graphviz_DIR})
    list (APPEND Graphviz_SEARCH_DIRS "ENV{Graphviz_DIR}" )
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

set(Graphviz_INCLUDE_DIRS ${Graphviz_INCLUDE_DIR})
get_filename_component(Graphviz_LIBRARY_DIR ${Graphviz_GVC_LIBRARY} PATH)
set ( TEST_SOURCE "#include <graphviz/gvc.h>\n#include <string>\n int main(){\nreturn strcmp(\"XX\",\"XXY\");\n}\n")
check_cxx_source_compiles("${TEST_SOURCE}" TEST_SOURCE_NOAST_COMPILES )
check_cxx_source_compiles("#define _PACKAGE_ast 1\n${TEST_SOURCE}" TEST_SOURCE_AST_COMPILES )
if (TEST_SOURCE_AST_COMPILES AND (NOT TEST_SOURCE_NOAST_COMPILES))
  set(Graphviz_DEFINES "-D_PACKAGE_ast=1")
else()
  set(Graphviz_DEFINES "")  
endif()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Graphviz FOUND_VAR Graphviz_FOUND REQUIRED_VARS Graphviz_DEFINES Graphviz_INCLUDE_DIR Graphviz_CGRAPH_LIBRARY Graphviz_GVC_LIBRARY  HANDLE_COMPONENTS)

mark_as_advanced(Graphviz_FOUND Graphviz_INCLUDE_DIRS Graphviz_CGRAPH_LIBRARY Graphviz_GVC_LIBRARY Graphviz_DEFINES)
