#--------
#-root IO

if(ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/rootIO/include ${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(rootIO_Classes include/rootIO_Classes.hh LINKDEF include/LinkDef.hh)

list(APPEND ROOT_FIND_COMPONENTS RIO)

#add_library(rootIO_ClassesDict SHARED rootIO_Classes.cxx ${sources})
#set(libsuffix .so)
#set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
#set_target_properties(rootIO_ClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})

#target_link_libraries(rootIO_ClassesDict HepMC ${ROOT_LIBRARIES})

add_library(rootIO SHARED ${PROJECT_SOURCE_DIR}/rootIO/src/IO_RootStreamer.cc rootIO_Classes.cxx)

#set(libsuffix .so)
#set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
#set_target_properties(rootIO PROPERTIES ${ROOT_LIBRARY_PROPERTIES})

target_link_libraries(rootIO ${ROOT_LIBRARIES} RIO HepMC)
set_target_properties(rootIO PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)


# installs 
install(TARGETS rootIO DESTINATION lib)
install(DIRECTORY include/HepMC DESTINATION include COMPONENT headers)
