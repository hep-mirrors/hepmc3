# building examples
#------------------
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
#

find_package(HepMC2 QUIET)

if(NOT HEPMC2_FOUND)
  message(STATUS "Validation: HEPMC2 package not found. Please check if HEPMC2_ROOT_DIR is set properly.") 
endif()

find_package(Pythia8 QUIET)

if(NOT PYTHIA8_FOUND)
  message(STATUS "Validation: Pythia8 package not found. Please check if PYTHIA8_ROOT_DIR is set properly.") 
endif()

find_package(Photos QUIET)

if(NOT PHOTOS_FOUND)
  message(STATUS "Validation: Photos package not found. Please check if PHOTOS_ROOT_DIR is set properly.") 
endif()

find_package(PhotosHepMC2 QUIET)

if(NOT PHOTOSHEPMC2_FOUND)
  message(STATUS "Validation: PhotosHepMC2 package not found. Please check if PHOTOSHEPMC2_ROOT_DIR is set properly.") 
endif()

# validation with HepMC3
add_executable(validation.exe ${PROJECT_SOURCE_DIR}/validation/validation.cc )
target_include_directories(validation.exe PRIVATE ${PROJECT_SOURCE_DIR}/include ${PHOTOS_INCLUDE_DIRS})
target_link_libraries(validation.exe ${PHOTOS_LIBRARIES} HepMC)
set_target_properties(validation.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)

# validation with HepMC2
add_executable(validation_HEPMC2.exe ${PROJECT_SOURCE_DIR}/validation/validation.cc )
target_include_directories(validation_HEPMC2.exe PRIVATE ${HEPMC2_INCLUDE_DIRS} ${PHOTOSHEPMC2_INCLUDE_DIRS})
target_link_libraries(validation_HEPMC2.exe ${PHOTOSHEPMC2_LIBRARIES} ${HEPMC2_LIBRARIES})
set_target_properties(validation_HEPMC2.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/bin)