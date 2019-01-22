
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/rootIO/include/  )
add_executable(pythia6_example.exe ${CMAKE_CURRENT_SOURCE_DIR}/main63.f ${CMAKE_CURRENT_SOURCE_DIR}/pythia-6.4.28.f ${CMAKE_CURRENT_SOURCE_DIR}/HepMC3Glue.cc)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../outputs/lib64/  ${CMAKE_CURRENT_SOURCE_DIR}/../outputs/lib/ )
if(ROOT_FOUND)
target_link_libraries(pythia6_example.exe  HepMC3 HepMC3rootIO)
else()
target_link_libraries(pythia6_example.exe  HepMC3)
endif()
set_target_properties(pythia6_example.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR})

install(TARGETS pythia6_example.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
