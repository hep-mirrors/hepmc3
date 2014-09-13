cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#----------------------------------------------------------------------------
project(HepMC3)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------

#---Compilation and Link options----------------------------------------------
if(CMAKE_SYSTEM_NAME MATCHES Darwin)
  set(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS} -flat_namespace -single_module -undefined dynamic_lookup")
endif()

set(CMAKE_CXX_FLAGS_DEBUG          "-g3 -fPIC")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -fPIC -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -fPIC -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2 -fPIC")
set(CMAKE_CXX_FLAGS                ${CMAKE_CXX_FLAGS_RELEASE})

# -pg
# -DBOOST_DISABLE_ASSERTS   
# -DBOOST_SP_DISABLE_THREADS

add_definitions(-DBOOST_DISABLE_ASSERTS)

include_directories(include)

#---Select sources for the various libraries---------------------------------
file(GLOB hepmc3_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB hepmc3_sources_IO ${PROJECT_SOURCE_DIR}/src/IO/*.cc)
file(GLOB hepmc3_sources_Search ${PROJECT_SOURCE_DIR}/src/Search/*.cc)

#---Add libraries------------------------------------------------------------

add_library(HepMC SHARED ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
add_library(HepMC_static STATIC ${hepmc3_sources} ${hepmc3_sources_IO} ${hepmc3_sources_Search})
set_target_properties(HepMC PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set_target_properties(HepMC_static PROPERTIES OUTPUT_NAME HepMC ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

#---Install build products---------------------------------------------------
install(TARGETS HepMC DESTINATION lib)
install(TARGETS HepMC_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
