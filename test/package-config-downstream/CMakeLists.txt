if(CMAKE_VERSION VERSION_LESS 4.0)
  # Keep CMP0057 unset to reproduce downstreams where IN_LIST is unavailable.
  cmake_minimum_required(VERSION 3.2)
else()
  cmake_minimum_required(VERSION 3.5)
endif()

project(HepMC3PackageConfigDownstream NONE)

find_package(HepMC3 REQUIRED)
