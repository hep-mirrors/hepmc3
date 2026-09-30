# Introduction to HepMC3

[HepMC3](http://hepmc.web.cern.ch/hepmc/) is the latest version of the
HepMC event record, the standard interface for writing and reading
particle-level event graphs from Monte Carlo parton-shower +
hadronisation event generators in particle and nuclear physics.

HepMC3 uses shared pointers for in-memory navigation and a
persistency-friendly [passive data
structure](https://en.wikipedia.org/wiki/Passive_data_structure) for
the event graph, addressing endemic issues with the previous HepMC1
and HepMC2 series. The current version additionally provides support
for [Les Houches Event Format partonic event records](https://arxiv.org/abs/hep-ph/0609017),
a Python interface, and multiple event-persistency formats.

Visit the [home page of the project](https://hepmc.web.cern.ch/) for
more information.  For brief information on the compatibility between
the HepMC3 versions, see below.

You can send bug reports, feature requests and questions about HepMC3
via the CERN GitLab repository https://gitlab.cern.ch/hepmc/HepMC3 or
by email to hepmc-devATcern.ch .


# Installing pre-built packages

HepMC is available from the LHC Computing Grid (LCG) package archives
(latest always available), in the Conda userspace packaging system,
and in several Linux-distribution package repositories
(version-support varies). Installation from source is also a
reasonable option in many use-cases.

## LCG platforms

The HepMC3 package is available for multiple LCG platforms, with the
latest versions in the latest LCG "view" version, `LCG_xx`,
cf. `/cvmfs/sft.cern.ch/lcg/views/LCG_*/`.  You can set up and use a
coherent LCG package stack easily from a HEP system with CVMFS access,
e.g.

```sh
$ . /cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc16-opt/setup.sh
$ HepMC3-config --version
3.03.01
```

For linking against HepMC3 with CMake, the `.cmake` files are located
under `$LCG_VIEW_DIR/share/HepMC3/cmake/`, e.g.
`/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc16-opt/share/HepMC3/cmake/`.

## Conda/Pixi

Pre-built HepMC can be installed in a user-level Conda environment
from the standard [conda-forge
repository](https://anaconda.org/channels/conda-forge/packages/hepmc3/overview):

```sh
conda install hepmc3
```

or

```sh
pixi add hepmc3
```

## Linux package repositories

For those with administrator access to their OS, HepMC3 is available
from the standard package-repositories of multiple Linux
distributions, currently Fedora, CentOS/EPEL, openSUSE, Arch,
Gentoo. (Note, the Debian/Ubuntu package is outdated: prefer another
source.)

The following commands will install HepMC3 in the corresponding
systems:

### Fedora
The package is available from the standard repository. To install:

```sh
sudo dnf install HepMC3 HepMC3-devel HepMC3-search HepMC3-search-devel HepMC3-interfaces-devel HepMC3-doc
```

To have a full installation of the HepMC3-doc package, add the option `--setopt=tsflags=''`

### RHEL, CentOS and compatible
The package is available from the EPEL repository. To install:

```sh
sudo yum install epel-release
sudo yum install HepMC3 HepMC3-devel HepMC3-search HepMC3-search-devel HepMC3-interfaces-devel HepMC3-doc
```

For Fedora and CentOS, the ROOT-interface packages `HepMC3-rootIO` and
`HepMC3-rootIO-devel` can be installed in the same way, but bring ROOT
as a package-dependency.  The Python binding packages have different
names depending on the platform, e.g.  `python36-HepMC3`, and
`python36-HepMC3-rootIO` for CentOS7. To install bindings for all the
available Python versions, you can use:

```sh
sudo yum install python*-HepMC3
```

### openSUSE/Leap
HepMC3 is available from the standard repositories
https://build.opensuse.org/package/show/openSUSE:Leap:15.2:Update/HepMC.
To install:

```sh
sudo zypper install HepMC3
```

This package does not include the ROOT interface.

### Arch and compatible
HepMC3 is available from
https://www.archlinux.org/packages/community/x86_64/hepmc/.  To
install:

```sh
sudo pacman -Syu hepmc
```

The dependencies can vary.

### Gentoo
The package is available in the standard repository
https://packages.gentoo.org/packages/sci-physics/hepmc.
To install:

```sh
sudo emerge --ask hepmc:3
```

## MacOSX
The HepMC3 package is available in the ``homebrew-hep``
repository https://davidchall.github.io/homebrew-hep/.
To install:

```sh
brew tap davidchall/hep
brew install hepmc3
```

The package optionally includes the ROOT interface.

## Windows
Precompiled HepMC3 packages are available for Windows and other
platforms via PyPI. Windows users can use `pip` to install HepMC3:

```sh
pip install HepMC3
```

The packages from `pip` do not include the ROOT interface.


# Build from source

If using pre-built HepMC3 installation from the repositories is not
possible or is not desired, you can build HepMC3 from the source. The
minimal requirements are:

- A C++ compiler with C++11 standard support.
- CMake version 3.X.


## Quick-start

It is strongly recommended to read this documentation completely
before the installation.  However, if for some reason that is not
possible, here is a set of commands for the installation that can
be copied and pasted into a Unix terminal with a few version-number
replacements for HepMC and Python. In some cases this action might
even result in a functional installation!

```sh
  wget http://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.Y.Z.tar.gz -O- | tar xz
  mkdir hepmc3-build
  cd hepmc3-build
  cmake -DCMAKE_INSTALL_PREFIX=../hepmc3-install   \
        -DHEPMC3_ENABLE_ROOTIO:BOOL=OFF            \
        -DHEPMC3_ENABLE_PROTOBUFIO:BOOL=OFF        \
        -DHEPMC3_ENABLE_TEST:BOOL=OFF              \
        -DHEPMC3_INSTALL_INTERFACES:BOOL=ON        \
        -DHEPMC3_BUILD_STATIC_LIBS:BOOL=OFF        \
        -DHEPMC3_BUILD_DOCS:BOOL=OFF     \
        -DHEPMC3_ENABLE_PYTHON:BOOL=ON   \
        -DHEPMC3_PYTHON_VERSIONS=3.14     \
        -DHEPMC3_Python_SITEARCH314=../hepmc3-install/lib/python3.14/site-packages \
        ../HepMC3-3.Y.Z/
  make
  make install
  ```

The full explanation of this is given on the dedicated [source-build
page](SRCBUILD.md)


# Installation troubleshooting

The possible problems during the HepMC3 installation can be caused by

 - A C++ compiler that does not support C++11.
   The only solution is to use compiler with C++11 support
 - The CMake version is too old.
 - Compilation of Python bindings fails. `pybind11`, which is used by
   HepMC3 to create the Python bindings supports only some compilers
   (`gcc`, `clang`, MSVC), therefore it is expected that the bindings
   will only work with these compilers.


#  Examples

HepMC3 is shipped with multiple example programs. These can be
compiled during installation as described above or after the
installation (for HepMC3 > 3.1.0). To compile the examples after the
installation copy the installed directory with examples to desired
directory and run CMake, e.g.
```sh
mkdir -p myexamples
cd myexamples
cp -r /usr/share/doc/HepMC3-3.2.6/examples ./
cd examples
cmake -DUSE_INSTALLED_HEPMC3=ON CMakeLists.txt
make
```

Please note that in case the HepMC3 installation is not system-wide,
CMake will require an additional flag `-D
HepMC3_DIR=/directory/where/you/have/HepMC3Config.cmake/file` to point
to the directory containing the installed `HepMC3Config.cmake` file.
The examples use multiple parts of HepMC3, therefore to use all of
them a full installation of HepMC3 (i.e. including ROOT MC event
generator interfaces is needed).


# Compatibility and deprecation notes

- The `IO_GenEvent` (HepMC2) and HEPEVT ASCII files produced by all
  HepMC3 versions should be readable by all HepMC3 versions and latest
  versions of HepMC2.
- The `Asciiv3` (HepMC3) ASCII files produced by all HepMC3 >= 3.0.0
  versions should be readable by all HepMC3 > 3.0.0 versions.
- The ROOT files produced by all HepMC3 >= 3.0.0 versions should be
  readable by all HepMC3 > 3.0.0 versions.
- The ROOT files produced by all HepMC3 >= 3.1.0 versions will not be
  readable by HepMC3 < 3.1.0 versions.

- The HepMC3 versions with the same `SOVERSION` of library are ABI
  backwards-compatible. i.e. code compiled with HepMC3 = 3.2.x will
  work with HepMC3 = 3.2.y if they have the same `SOVERSION`.  Please
  note that `libHepMC3`, `libHepMC3search` and other libraries have
  different `SOVERSION`s.
- The minor versions of HepMC3 are API backward compatible.
- The major versions of HepMC3 are almost API backward compatible.

- For HepMC3 > 3.2.6 the member functions in `GenEvent/GenVertex` that
  deal with raw pointers, e.g. `add_particle_in ( GenParticle *p )`
  will be removed.
- Python2 is not in development anymore, therefore it was removed from
  HepMC3 > 3.2.7.
- The class `HepMC3::RelativesInterface` is deprecated and will be
  removed in the future.  Use `HepMC3::children_particles`,
  `HepMC3::descendant_particles`, etc. instead.
- The minimal required version of `cmake` slowly changes from version
  to version. It is recommended to use a recent `cmake`.
- LCG uses the GNU and LLVM (Clang) compiler toolsets on
  `Linux/x86_64`, therefore those are the primary supported compilers.
  In addition to that HepMC3 compilation is tested regularly with
  `MSVC@Windows/x86_64`, `GNU@Darwin/x86_64`, `GNU@Darwin/aarch64` and
  `GNU@Linux/(architectures supported by RHEL/Fedora)`. The following
  compiler combinations are tested irregularly: `ARM@Linux/aarch64`,
  `IntelLLVM@Linux/x86_64`, `Intel@Linux/x86_64`,
  `NVidia@Linux/x86_64`, `IBM XL@Linux/ppc64le`, and
  `AOCC@Linux/x86_64`. `SunPro@Linux/x86_64` is not supported as of
  3.2.8.


# Building and Running the ConvertExample with the output EDM4HEP

Set up your favorite KEY4HEP nightly:
```sh
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```
or release:
```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
```

Then build with KEY4HEP by adding the following flag to the `cmake`
command:
```
-D HEPMC3_ENABLE_EDM4HEP=ON -D HEPMC3_BUILD_EXAMPLES=ON
```
This flag will initiate some sanity checks. The converter resides in
the example programs, therefore these also have to be turned on during
the build.
