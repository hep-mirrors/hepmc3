# Introduction
HepMC3 is a new rewrite of HepMC event record. It uses shared pointers for in-memory navigation and the POD concept for the persistency. 

# A Quick Start:

1. Checkout the HepMC from GIT repository:
  ```
  git clone hhttps://gitlab.cern.ch/hepmc/HepMC3.git
  ```
  
2. Create a workspace area on which to perform the builds 
  ```
  mkdir hepmc3-build
  cd hepmc3-build
  ```
  
3. Configure the build of the HepMC3
  ```
  cmake -DHEPMC_ENABLE_ROOTIO=OFF -DCMAKE_INSTALL_PREFIX=../hepmc3-install ../HepMC3 
  ```
  
4. In order to build with ROOTIO put: 
  ```
  -DHEPMC_ENABLE_ROOTIO=ON -DROOTDIR=path_to_ROOT6_installation
  ``` 
  
5. In order to build with HepMC3 example put
  ```
  -DHEPMC_BUILD_EXAMPLES=ON 
  ``` 
  
6. Build and install HepMC3
  ```
  make -jN install
  ```
  

