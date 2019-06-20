set(CMAKE_MODULE_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules    ${CMAKE_MODULE_PATH})
find_package(Graphviz)
if(GRAPHVIZ_FOUND)
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree Gui Graf Hist Gpad )
include_directories(${PROJECT_SOURCE_DIR}/rootIO/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}  ${ROOT_INCLUDE_DIRS} ${GRAPHVIZ_INCLUDE_DIRS} ./)

ROOT_GENERATE_DICTIONARY(HepMC3ViewerFrameDict include/HepMC3ViewerFrame.h LINKDEF include/LinkDef.h OPTIONS -inlineInputHeader)


ADD_EXECUTABLE(HepMC3Viewer.exe  src/main.cxx  src/HepMC3ViewerFrame.cxx HepMC3ViewerFrameDict.cxx  )
set_target_properties(HepMC3Viewer.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR})
include_directories( include  )
if (GRAPHVIZ_GRAPH_LIBRARY)
TARGET_LINK_LIBRARIES(HepMC3Viewer.exe  ${ROOT_LIBRARIES}  ${GRAPHVIZ_GRAPH_LIBRARY}  ${GRAPHVIZ_CGRAPH_LIBRARY} ${GRAPHVIZ_GVC_LIBRARY} HepMC3 HepMC3rootIO  )
else()
TARGET_LINK_LIBRARIES(HepMC3Viewer.exe  ${ROOT_LIBRARIES}  ${GRAPHVIZ_CGRAPH_LIBRARY} ${GRAPHVIZ_GVC_LIBRARY} HepMC3 HepMC3rootIO  )
endif()

install(TARGETS HepMC3Viewer.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${CMAKE_INSTALL_BINDIR} FILES_MATCHING PATTERN "*.pcm" PATTERN "*config"  PATTERN "CMakeFiles" EXCLUDE )
install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/${CMAKE_INSTALL_BINDIR}/ DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILES_MATCHING PATTERN "*env.sh*")
        
elseif()
  message(STATUS "Examples: Graphviz package not found. ")
  message(STATUS "ViewExample disabled")
endif()


#  GRAPHVIZ_INCLUDE_DIRS - Graphviz include directories
#  GRAPHVIZ_CDT_LIBRARY - Graphviz CDT library
#  GRAPHVIZ_GVC_LIBRARY - Graphviz GVC library
#  GRAPHVIZ_CGRAPH_LIBRARY -