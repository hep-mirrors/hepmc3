# HepMC3 source-build instructions

## Requirements

The minimal requirements are:

- A C++ compiler with C++11 standard support.
- CMake version >= 3.10.


## Quick-start

It is strongly recommended to read this documentation completely
before the installation.  However, if for some reason that is not
possible, here is a set of commands for the installation that can be
copied and pasted into a Unix terminal (updating the HepMC version
in the first line if needed):

```sh
  VERSION=3.3.2
  PYVERSION=$(python -c 'import sys; print(f"{sys.version_info.major}.{sys.version_info.minor}")')
  wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-$VERSION.tar.gz -O- | tar xz
  mkdir hepmc3-build
  cd hepmc3-build
  cmake -DCMAKE_INSTALL_PREFIX=../hepmc3-install   \
        -DHEPMC3_ENABLE_ROOTIO=OFF            \
        -DHEPMC3_ENABLE_PROTOBUFIO=OFF        \
        -DHEPMC3_ENABLE_TEST=OFF              \
        -DHEPMC3_INSTALL_INTERFACES=ON        \
        -DHEPMC3_BUILD_STATIC_LIBS=OFF        \
        -DHEPMC3_BUILD_DOCS=OFF     \
        -DHEPMC3_ENABLE_PYTHON=ON   \
        -DHEPMC3_PYTHON_VERSIONS=$PYVERSION   \
        -DHEPMC3_Python_SITEARCH${PYVERSION/./}=../hepmc3-install/lib/python$PYVERSION/site-packages \
        ../HepMC3-$VERSION/
  make
  make install
  ```

## Detailed instructions

1. The first step of the installation is to
  a) Check out HepMC3 from the `git` repository:
  ```sh
  git clone https://gitlab.cern.ch/hepmc/HepMC3.git
  ```
  or b) download the tarball with `wget` or any other tool:
  ```sh
  wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.Y.Z.tar.gz
  tar -xzf HepMC3-3.Y.Z.tar.gz
  ln -s HepMC3-3.Y.Z HepMC3
  ```
  The final command here is not essential, it just allows us to write the rest
  of this guide on the assumption that your sources live in the `HepMC3/` directory.


2. Create a workspace area on which to perform the builds:
  ```sh
  mkdir hepmc3-build
  cd hepmc3-build
  ```
  You might like to use another build-dir name to match up with your versioned
  source directory.


3. Configure the build and installation with CMake; the simplest command is:

  ```sh
  cmake ../HepMC3/ -D CMAKE_INSTALL_PREFIX=/path/to/installation/
  ```

  (Note that by default the ROOT I/O plugin is enabled, and this
  `cmake` command requires that your ROOT installation is findable.
  To disable ROOT I/O, add `-D HEPMC3_ENABLE_ROOTIO=OFF` to the command.)

  On some systems the appropriate CMake executable is called `cmake3`
  or similar.  Note that you have to have permissions to install
  HepMC3 into the indicated directory.

  You can add further flags to the `cmake` command-line to add/remove
  HepMC3 optional features:

  - To build the example programs, add
    ```sh
    -D HEPMC3_BUILD_EXAMPLES=ON
    ```
    Some examples are disabled on Windows.

  - To build test programs, add
    ```sh
    -D HEPMC3_ENABLE_TEST=ON
    ```
    After the build, run
    ```sh
    make test
    ```
    or
    ```sh
    ctest .
    ```
    On Windows/MSVC the build type should be specified:
    ```sh
    ctest . -C Debug
    ```
    HepMC3 uses extra packages for tests and for development purposes, e.g.
    graphviz or binder. On Windows, most extra packages have no default location
    and may require extra specification.

  - To disable compilation of the `libHepMC3search` search engine (enabled by
  default), add
    ```sh
    -D HEPMC3_ENABLE_SEARCH=OFF
    ```

  - To disable compilation of Python bindings (enabled by default) add
    ```sh
    -D HEPMC3_ENABLE_PYTHON=OFF
    ```
    See below for more details on configuring the Python extension against non-system
    Python environments (e.g. in a virtual environment) and for compatibility with PyPy.


  - ROOT I/O is assumed on by default, but to specify the ROOT installation
    to use, add the following flags:
    ```sh
    -D HEPMC3_ENABLE_ROOTIO=ON -DROOT_DIR=/path/to/ROOT/installation/
    ```
    and define `LD_LIBRARY_PATH` (`DYLD_LIBRARY_PATH` on Mac OS X):
    ```sh
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/path/to/ROOT6/libraries/
    ```

    To *disable* ROOT, add `-D HEPMC3_ENABLE_ROOTIO=OFF` to the default
    `cmake` call.

  - To build with PROTOBUF I/O add the following flags:
    ```sh
    -D HEPMC3_ENABLE_PROTOBUFIO=ON
    ```
    Adjustment of `LD_LIBRARY_PATH`/`DYLD_LIBRARY_PATH` might be needed.


4. Build and install:
  ```sh
  make -jN install
  ```
  where `N` denotes a number of desired parallel jobs.

  On Windows (in Unix-compatible shell) one can use
  ```
  cmake --build ./
  ```
  instead.

  To build a Windows installer use NSIS and run
  ```sh
  cpack.exe -G NSIS -C Debug
  ```

  The primary supported development chains are
  gcc@Linux, clang@Linux, gcc@OSX, clang@OSX and  MSVC@Windows10, however
  the code has been tested on other compilers and systems, e.g. clang@FreeBSD,
  Sun@Solaris11, gcc@Solaris11, Sun@Linux, PGC@Linux etc.
  Only a limited support can be offered for these systems.


## Installing non-system Python bindings

HepMC3 is supplied with Python-binding codes that can be build on
multiple systems.  The number of potential combinations of compiler
suites, Python versions and operation systems is quite large,
therefore this part of the build from source may require careful configuration.
The pre-built packages and [PyPI package](https://pypi.org/project/HepMC3/)
already set up their Python bindings appropriately.

The installation path for the Python modules can be tweaked with a set
of version-specific CMake variables with the structure:
```sh
HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR}
```
For example, the variable is called `HEPMC3_Python_SITEARCH36` for Python
version 3.6, and to set the installation path of the HepMC3 modules you would
add this to the arguments of `cmake`:
```sh
-D HEPMC3_Python_SITEARCH36=/path/to/lib/python3.6/modules/
```

By default, the
`HEPMC3_Python_SITEARCH${Python_VERSION_MAJOR}${Python_VERSION_MINOR}`
variables are set to the path of the system installation of Python,
which may be outside the HepMC3 installation directory `$CMAKE_INSTALL_DIR`.
(Python and Conda virtual environments may change this behaviour.) If you do
not have permissions to install the Python modules to the system-Python
module directory, manual specification is mandatory; a natural choice is
the Python module directory inside your installation prefix, e.g.
```sh
cmake ...
   -D CMAKE_INSTALL_PREFIX=/my/install/path
   -D HEPMC3_Python_SITEARCH36=/my/install/path/lib/python3.6/site-packages
```

To specify the desired versions of Python against which to build the
HepMC3 modules, pass the `HEPMC3_PYTHON_VERSIONS` option to `cmake`, e.g.
```sh
cmake ... -D HEPMC3_PYTHON_VERSIONS=3.6,3.12
```
will build Python modules for versions Python 3.6 and Python 3.12.
By default CMake will attempt to build the Python modules for the latest
installed Python version.


## Modules for PyPy

In addition to the standard CPython modules, it is possible to build
HepMC3 modules for PyPy. However, the PyPy support is experimental. To
build the bindings against the `pypy-c` library use `pypy<version>`
for the `HEPMC3_PYTHON_VERSIONS` option, e.g.
```sh
-D HEPMC3_PYTHON_VERSIONS=pypy3
```
This also requires quite a recent CMake.


## MC-generator interfaces

HepMC3 is shipped with interfaces to some legacy MC event
generators/codes located in the interfaces/ directory, to allow the
usage of HepMC3 with codes that do not have native HepMC3
interfaces.

To enable the installation of interfaces use
`-D HEPMC3_INSTALL_INTERFACES:BOOL=ON` option for `cmake`.
