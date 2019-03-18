macro( hepmc_test testname )
  message( STATUS "building ${testname} for ${testname}" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}   ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
  target_link_libraries(${testname} PUBLIC  PRIVATE HepMC3 )
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${CMAKE_CURRENT_BINARY_DIR}/${testname} )
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_test )

macro( hepmc_root_test testname )
  message( STATUS "building ROOT test ${testname} " )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/ ${CMAKE_CURRENT_SOURCE_DIR}/../rootIO/include/)
  target_link_libraries(${testname} HepMC3 HepMC3rootIO)
   
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_root_test )










set( CMAKE_INCLUDE_DIRECTORIES_BEFORE ON )

file(GLOB INPUTFILES  "${CMAKE_CURRENT_SOURCE_DIR}/*.input" )
file(COPY ${INPUTFILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set( HepMC_tests
        testMass
        testPrintBug
        testPolarization 
        testUnits
        testMultipleCopies
        testWeights 
        testLoops
        testDelete
        testIO1
        testIO3
        testIO6
        testReaderFactory1
         )

set( HepMC_root_tests 
        testIO2
        testIO4
        testReaderFactory2
        )


set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(Threads)
if(Threads_FOUND)
  list( APPEND HepMC_tests "testThreads1" )   
else()
  message(STATUS "No threads library found, testThreads1 is disabled")
endif()

foreach ( test ${HepMC_tests} )
  hepmc_test( ${test} )
  set_tests_properties(${test} PROPERTIES TIMEOUT 20)  #Relevant for testLoops
endforeach ( test ${HepMC_tests} )

if(Threads_FOUND)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(testThreads1 PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(testThreads1  "${CMAKE_THREAD_LIBS_INIT}")
endif()
endif()

if(HEPMC3_ENABLE_ROOTIO)
foreach ( test ${HepMC_root_tests} )
  hepmc_root_test( ${test} )
endforeach ( test ${HepMC_root_tests} )
target_link_libraries(testIO4 ${ROOT_Hist_LIBRARY})
endif()



set( libMCEG_sources ValidationControl.cc   SimpleEventTool.cc )
set( libMCEG_sources_HEPMC3  )
set( libMCEG_link_libraries_HEPMC2  )
set( libMCEG_link_libraries_HEPMC3  )
set( libMCEG_include_directories   ${CMAKE_CURRENT_SOURCE_DIR}/../search/include/  )
set( libMCEG_compile_definitions  "-DSIMPLEEVENTTOOL")
set( MCEG_tests )


find_package(Pythia8 )
if(NOT PYTHIA8_FOUND)
  message(STATUS "Validation: Pythia8 package not found. Please check if PYTHIA8_ROOT_DIR is set properly.") 
else()
  message( STATUS "building testPythia for testPythia" )
  list( APPEND libMCEG_sources    "PythiaValidationTool.cc" )
  list( APPEND libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/pythia8/src/Pythia8ToHepMC3.cc"    )
  list( APPEND libMCEG_link_libraries_HEPMC2  "${PYTHIA8_LIBRARIES}")
  list( APPEND libMCEG_link_libraries_HEPMC3  "${PYTHIA8_LIBRARIES}")
  list( APPEND libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/pythia8/include"   "${PYTHIA8_INCLUDE_DIRS}")
  list( APPEND libMCEG_compile_definitions  "-DPYTHIA8")

  list( APPEND MCEG_tests "testPythia1" )   
  list( APPEND MCEG_tests "testPythia2" )   
endif()


find_package(Photospp )
if(NOT PHOTOSPP_FOUND)
  message(STATUS "Validation: Photospp package not found. Please check if PHOTOSPP_ROOT_DIR is set properly.") 
else()
  message( STATUS "building testPhotos tests" )
  message(STATUS "PHOTOSPP_LIBRARIES ${PHOTOSPP_LIBRARIES}")
  message(STATUS "PHOTOSPP_HEPMC2_LIBRARIES ${PHOTOSPP_HEPMC2_LIBRARIES}")
  message(STATUS "PHOTOSPP_INCLUDE_DIRS ${PHOTOSPP_INCLUDE_DIRS}")
  list( APPEND  libMCEG_sources   "PhotosValidationTool.cc" )
  list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/Photospp/src/PhotosHepMC3Event.cc" "${PROJECT_SOURCE_DIR}/interfaces/Photospp/src/PhotosHepMC3Particle.cc"   )
  list( APPEND  libMCEG_link_libraries_HEPMC2  "${PHOTOSPP_LIBRARIES}"  "${PHOTOSPP_HEPMC2_LIBRARIES}")
  list( APPEND  libMCEG_link_libraries_HEPMC3  "${PHOTOSPP_LIBRARIES}" )
  list( APPEND  libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/Photospp/include"  "${PHOTOSPP_INCLUDE_DIRS}")
  list( APPEND  libMCEG_compile_definitions  "-DPHOTOSPP")

  list( APPEND  MCEG_tests "testPhotos1" )   
endif()


find_package(Tauolapp )
if(NOT TAUOLAPP_FOUND)
  message(STATUS "Validation: Tauolapp package not found. Please check if TAUOLA_ROOT_DIR is set properly.") 
else()
  message( STATUS "building testTauola tests" )
  message(STATUS "TAUOLAPP_LIBRARIES ${TAUOLAPP_LIBRARIES}")
  message(STATUS "TAUOLAPP_INCLUDE_DIRS ${TAUOLAPP_INCLUDE_DIRS}")
 list( APPEND  libMCEG_sources "TauolaValidationTool.cc" )
 list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/src/TauolaHepMC3Event.cxx" "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/src/TauolaHepMC3Particle.cxx"  )
 list( APPEND  libMCEG_link_libraries_HEPMC2  "${TAUOLAPP_LIBRARIES}" )
 list( APPEND  libMCEG_link_libraries_HEPMC3  "${TAUOLAPP_LIBRARIES}" )
 list( APPEND  libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/Tauolapp/include"  "${TAUOLAPP_INCLUDE_DIRS}") 
 list( APPEND  libMCEG_compile_definitions  "-DTAUOLAPP")
 list( APPEND  MCEG_tests "testTauola1" )   
endif()



if(HEPMC3_ENABLE_ROOTIO)
find_package(MCTester )
if(NOT MCTESTER_FOUND)
  message(STATUS "Validation: MCTester package not found. Please check if MCTESTER_ROOT_DIR is set properly.") 
else()
  message( STATUS "building testMCTester tests" )
  message(STATUS "MCTESTER_LIBRARIES ${MCTESTER_LIBRARIES}")
  message(STATUS "MCTESTER_INCLUDE_DIRS ${MCTESTER_INCLUDE_DIRS}")
  list( APPEND  libMCEG_sources "McTesterValidationTool.cc" )
  list( APPEND  libMCEG_sources_HEPMC3 "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/src/HepMC3Event.cxx" "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/src/HepMC3Particle.cxx"   )
  list( APPEND  libMCEG_link_libraries_HEPMC2  "${MCTESTER_LIBRARIES}" "${ROOT_LIBRARIES}" )
  list( APPEND  libMCEG_link_libraries_HEPMC3  "${MCTESTER_LIBRARIES}" "${ROOT_LIBRARIES}" )
  list( APPEND  libMCEG_include_directories  "${PROJECT_SOURCE_DIR}/interfaces/mc-tester/include" "${MCTESTER_INCLUDE_DIRS}"  "${ROOT_INCLUDE_DIRS}")
  list( APPEND  libMCEG_compile_definitions  "-DMCTESTER")

  list( APPEND  MCEG_tests "testMCTester1" )   
endif()
endif()


add_library(MCEG3 SHARED ${libMCEG_sources} ${libMCEG_sources_HEPMC3})
target_link_libraries(MCEG3  PRIVATE HepMC3 HepMC3search PUBLIC ${libMCEG_link_libraries_HEPMC3} )
target_compile_definitions(MCEG3  PRIVATE ${libMCEG_compile_definitions})
target_include_directories(MCEG3   BEFORE
PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
PRIVATE ${libMCEG_include_directories}
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/)



macro( hepmc_mceghepmc3_test testname )
  message( STATUS "building MCEG3 test ${testname} with HEPMC3" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source}  ${ARGN} )
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/  ${CMAKE_CURRENT_SOURCE_DIR}/../search/include/ )
  #The flag is needed for Taoula that conflates generic C++ interface with the HepMC2 interface
  #FIXME put here Visual studio flags when needed
  #For Darwin target_link_libraries(${testname}  "-undefined dynamic_lookup"  HepMC3  MCEG3)   
if (APPLE)  
  target_link_libraries(${testname}  "-undefined dynamic_lookup"  HepMC3  MCEG3)   
else()
  target_link_libraries(${testname}  "-Wl,--allow-shlib-undefined"  HepMC3  MCEG3)   
endif()
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( ${testname} ${testname} )
  file(GLOB INPUT${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUT${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_mceghepmc3_test )


foreach ( test ${MCEG_tests} )
  hepmc_mceghepmc3_test( ${test} )
endforeach ( test ${MCEG_tests} )





find_package(HepMC2 QUIET)

if(NOT HEPMC2_FOUND)
  message(STATUS "HEPMC2 package not found. Please check if HEPMC2_ROOT_DIR is set properly.") 

else()

add_library(MCEG2 SHARED ${libMCEG_sources})
target_link_libraries(MCEG2  PRIVATE ${HEPMC2_LIBRARIES} PUBLIC ${libMCEG_link_libraries_HEPMC2} )
target_compile_definitions(MCEG2  PRIVATE ${libMCEG_compile_definitions} -DHEPMC2)
target_include_directories(MCEG2   BEFORE
PUBLIC ${HEPMC2_INCLUDE_DIRS}
PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
PRIVATE ${libMCEG_include_directories}
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   )


macro( hepmc_mceghepmc2_test testname )
  message( STATUS "building MCEG2 test ${testname} with HEPMC2" )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(hepmc2_${testname} ${${testname}_source}  ${ARGN} )
  target_compile_definitions(hepmc2_${testname}  PRIVATE  -DHEPMC2)
  target_include_directories(${testname} BEFORE
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src   ${CMAKE_CURRENT_SOURCE_DIR}/../include/  ${CMAKE_CURRENT_SOURCE_DIR}/../search/include/ )  
  target_link_libraries(hepmc2_${testname} ${HEPMC2_LIBRARIES}   MCEG2   )
  set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
  add_test( hepmc2_${testname} hepmc2_${testname} )
  file(GLOB INPUThepmc2_${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUThepmc2_${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro( hepmc_mceghepmc2_test )



foreach ( test ${MCEG_tests} )
  hepmc_mceghepmc2_test( ${test} )
endforeach ( test ${MCEG_tests} )

endif()

#One should use something else for Windows or use build-in Linux sybsystem
find_program( MEMORYCHECK_COMMAND valgrind )
if(MEMORYCHECK_COMMAND)
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --error-exitcode=1" )
endif()

macro(add_memcheck_test binaryname )
  set(memcheck_command "${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}")
  separate_arguments(memcheck_command)
  add_test(memcheck_${binaryname} ${memcheck_command} ./${binaryname} ${ARGN})
  file(GLOB INPUTmemcheck_${testname}  "${CMAKE_CURRENT_SOURCE_DIR}/input${testname}*" )
  file(COPY ${INPUTmemcheck_${testname}} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro(add_memcheck_test)


if(MEMORYCHECK_COMMAND)
foreach ( test ${HepMC_tests} )
  add_memcheck_test( ${test} )
  set_tests_properties(memcheck_${test} PROPERTIES DEPENDS ${test})
endforeach ( test ${HepMC_tests} )
endif()

