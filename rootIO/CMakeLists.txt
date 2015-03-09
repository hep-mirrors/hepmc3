#--------
#-root IO

if(ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/rootIO/include ${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Generate dictionaries
#
ROOT_GENERATE_DICTIONARY(rootIO_Classes rootIO_Classes.hh LINKDEF LinkDef.hh OPTIONS -inlineInputHeader)

list(APPEND ROOT_FIND_COMPONENTS RIO)

add_library(rootIO SHARED ${PROJECT_SOURCE_DIR}/rootIO/src/IO_Root.cc rootIO_Classes.cxx)

target_link_libraries(rootIO ${ROOT_LIBRARIES} RIO HepMC)
set_target_properties(rootIO PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/lib)


# installs 
install(TARGETS rootIO DESTINATION lib)
install(DIRECTORY ${PROJECT_BINARY_DIR}/rootIO/ DESTINATION lib FILES_MATCHING PATTERN "*.pcm")
install(DIRECTORY include/HepMC DESTINATION include COMPONENT headers)
