if (USE_INSTALLED_HEPMC3)
#This version is for standalone compilation of examples.
option(USE_INTERFACE_FROM_PYTHIA8 "Use Pythia8/Pythia8ToHepMC3.h from Pythia8" OFF)
set(CMAKE_MODULE_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules    ${CMAKE_MODULE_PATH})
find_package(Pythia8 QUIET)
if(PYTHIA8_FOUND)
message(STATUS "HepMC3 Pythia8Example: Pythia8 package found. pythia8_example enabled")
SET(interfacelocation ${HEPMC3_INTERFACES_DIR}/pythia8/include/Pythia8)
if (USE_INTERFACE_FROM_PYTHIA)
find_file(interfacefrompythia8 ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugins/Pythia8ToHepMC3.h)
if (interfacefrompythia8) 
message(STATUS "HepMC3 Pythia8Example: Found Pythia8Plugins/Pythia8ToHepMC3.h  in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
else()
message(FATAL_ERROR "HepMC3 Pythia8Example: Pythia8Plugins/Pythia8ToHepMC3.h  is absent in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
endif()
SET(interfacelocation ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugings)
endif()
include_directories(${PROJECT_SOURCE_DIR}/include ${interfacelocation} ${PYTHIA8_INCLUDE_DIR})
add_executable(pythia8_example.exe ${CMAKE_CURRENT_SOURCE_DIR}/pythia8_example.cc )
endif()
else()
#This version is for compilation during installation. Should be consistent with Pythia used in tests
set(PYTHIA8_ROOT_DIR "" CACHE PATH "Location of Pythia8 installation.")
find_package(Pythia8 QUIET)
if(PYTHIA8_FOUND)
SET(interfacelocation ${PROJECT_SOURCE_DIR}/interfaces/pythia8/include/Pythia8)
if (USE_INTERFACE_FROM_PYTHIA)
find_file(interfacefrompythia8 ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugins/Pythia8ToHepMC3.h)
if (interfacefrompythia8) 
message(STATUS "HepMC3 Pythia8Example: Found Pythia8Plugins/Pythia8ToHepMC3.h  in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
else()
message(FATAL_ERROR "HepMC3 Pythia8Example: Pythia8Plugins/Pythia8ToHepMC3.h  is absent in Pythia8 installation in ${PYTHIA8_INCLUDE_DIR}")
endif()
SET(interfacelocation ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugings)
endif()
find_file(interfacefrompythia8 ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugins/Pythia8ToHepMC3.h)
include_directories(${PROJECT_SOURCE_DIR}/include ${interfacelocation} ${PYTHIA8_INCLUDE_DIR})
add_executable(pythia8_example.exe ${CMAKE_CURRENT_SOURCE_DIR}/pythia8_example.cc )
endif()
endif()


if(PYTHIA8_FOUND)
target_link_libraries(pythia8_example.exe ${PYTHIA8_LIBRARIES} HepMC3)
set_target_properties(pythia8_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR})

# create environment scripts

set(bindir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
get_filename_component(PYTHIA8_LIB_DIR ${PYTHIA8_LIBRARY} PATH)
set(libdir "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}:${PYTHIA8_LIB_DIR}")
set(pythia8data "${PYTHIA8_XMLDOC_DIR}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pythia8_example_env.sh.in
	       ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/pythia8_example_env.sh
     	       @ONLY
      	       )
# installs 
if (USE_INSTALLED_HEPMC3)
install(TARGETS pythia8_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES pythia8_ee_to_Z_to_tautau.conf DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILES_MATCHING PATTERN "*env.sh*")
endif()
else()
  message(STATUS "HepMC3 examples: Pythia8 package not found. Please check if PYTHIA8_ROOT_DIR is set properly. pythia8_example disabled")
endif()
