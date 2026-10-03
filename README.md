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
for [Les Houches Event Format partonic event
records](https://arxiv.org/abs/hep-ph/0609017), a Python interface,
and multiple event-persistency formats.

Visit the [home page of the project](https://hepmc.web.cern.ch/) for
more information.  For brief information on the compatibility between
the HepMC3 versions, see below.

You can send bug reports, feature requests and questions about HepMC3
via the CERN GitLab repository https://gitlab.cern.ch/hepmc/HepMC3 or
by email to hepmc-devATcern.ch .

HepMC3 can be installed from multiple pre-built package repositories
or installed from source, as described in the following sections.


## Installing pre-built HepMC3 packages

HepMC3 is available through several pre-built software distributions
and package managers, including the [LHC Computing Grid
(LCG)](https://home.cern/science/computing/grid/),
[Conda](https://docs.conda.io/projects/conda/en/latest/index.html),
several Linux-distribution package repositories, and installers for
Mac OS X and Windows. The available HepMC3 version depends on the
distribution or repository and may not include the latest version.

### LCG platforms

HepMC3 is included in several LCG software stacks distributed through
CERN SFT CVMFS (`cvmfs-sft.cern.ch`). If your system has access to
CVMFS, you can use HepMC3 by setting up an appropriate LCG view, for
example:

```sh
$ . /cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc16-opt/setup.sh
$ HepMC3-config --version
3.03.01
```

For linking against HepMC3 with CMake, the `.cmake` files are located
under `$LCG_VIEW_DIR/share/HepMC3/cmake/`, e.g.
`/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc16-opt/share/HepMC3/cmake/`.

### Conda/Pixi

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

### Linux package repositories

HepMC3 is available from the standard package-repositories of multiple
Linux distributions, currently Fedora, CentOS/EPEL, openSUSE, Arch,
Gentoo. (Note, the Debian/Ubuntu package is outdated: prefer another
source.)

This approach usually requires system-administrator permissions. With
appropriate permissions, the following commands will install HepMC3 in
each system:

#### Fedora
HepMC3 is available from the standard repository. To install:

```sh
sudo dnf install HepMC3 HepMC3-devel HepMC3-search HepMC3-search-devel HepMC3-interfaces-devel HepMC3-doc
```

To have a full installation of the HepMC3-doc package, add the option `--setopt=tsflags=''`

#### RHEL and compatible
HepMC3 is available from the EPEL repository. To install:

```sh
sudo yum install epel-release
sudo yum install HepMC3 HepMC3-devel HepMC3-search HepMC3-search-devel HepMC3-interfaces-devel HepMC3-doc
```

For Fedora and RHEL-compatible distributions, the ROOT-interface
packages `HepMC3-rootIO` and `HepMC3-rootIO-devel` can be installed in
the same way, but bring ROOT as a package-dependency.  The Python
binding packages have different names depending on the platform. To
install bindings for all the available Python versions, you can use:

```sh
sudo yum install python*-HepMC3
```

#### openSUSE/Leap
HepMC3 is available from the standard repositories
https://build.opensuse.org/package/show/openSUSE:Leap:15.2:Update/HepMC.
To install:

```sh
sudo zypper install HepMC3
```

This package does not include the ROOT interface.

#### Arch and compatible
HepMC3 is available from
https://aur.archlinux.org/packages/hepmc .  To
install:

```sh
sudo pacman -Syu hepmc
```

The dependencies can vary.

#### Gentoo
HepMC3 is available in the standard repository
https://packages.gentoo.org/packages/sci-physics/hepmc.
To install:

```sh
sudo emerge --ask hepmc:3
```

### MacOSX

HepMC3 is available in the ``homebrew-hep`` repository
https://davidchall.github.io/homebrew-hep/.  To install:

```sh
brew tap davidchall/hep
brew install hepmc3
```

The package optionally includes the ROOT interface.

### Windows

Precompiled HepMC3 packages are available for Windows and other
platforms via PyPI. Windows users can use `pip` to install HepMC3:

```sh
pip install HepMC3
```

The packages from `pip` do not include the ROOT interface.


## Build from source

If using pre-built HepMC3 installation from the repositories is not
possible or is not desired, you can build HepMC3 from the source. For
full instructions, see [the dedicated page](SRCBUILD.md).


## Installation troubleshooting

The possible problems during the HepMC3 installation can be caused by

 - A C++ compiler that does not support C++11.
   The only solution is to use compiler with C++11 support
 - The CMake version is too old.
 - Compilation of Python bindings fails. `pybind11`, which is used by
   HepMC3 to create the Python bindings supports only some compilers
   (`gcc`, `clang`, MSVC), therefore it is expected that the bindings
   will only work with these compilers.


## Event standardisation

HepMC3 event objects can create very general event representations. In
order to be directly useable by tools in the standard HEP MC
ecosystem, events should also follow standards on structure and
standard codes and naming of particle IDs, statuses, and
event-weights.

See the [standards](STANDARDS.md) page for full information on
standard conventions that HepMC events are expected to respect.


##  Usage examples

HepMC3 is shipped with multiple example programs. These can be
compiled during installation as described above or after the
installation (for HepMC3 > 3.1.0). To compile the examples after the
installation copy the installed directory with examples to desired
directory and run CMake, e.g.
```sh
mkdir -p myexamples
cd myexamples
cp -r /usr/share/doc/HepMC3-3.3.2/examples ./
cd examples
cmake -D USE_INSTALLED_HEPMC3=ON CMakeLists.txt
make
```

If CMake cannot locate your HepMC3 installation automatically, add `-D
HepMC3_DIR=/path/to/hepmc3/cmake` to your CMake configuration
command. Replace the placeholder with the path to the directory
containing the installed `HepMC3Config.cmake` file.

The examples use different HepMC3 components and external
dependencies. To build and run all examples, install the required
optional HepMC3 components, including ROOT I/O and the search library,
together with the external dependencies required by individual
examples, such as ROOT and the relevant Monte Carlo event generators.

### Building and Running the `ConvertExample` with the output EDM4HEP

Set up your favorite Key4hep nightly:
```sh
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```
or release:
```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
```

Then build with Key4hep by adding the following flag to the `cmake`
command:
```
-D HEPMC3_ENABLE_EDM4HEP=ON -D HEPMC3_BUILD_EXAMPLES=ON
```
This flag will initiate some sanity checks. The converter resides in
the example programs, therefore these also have to be turned on during
the build.


## Compatibility and deprecation notes

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

- Deprecated functions that operated on raw pointers were removed in
  HepMC3 3.3.0. For example, the
  `GenVertex::add_particle_in(GenParticle*)` overload was removed; use
  the shared-pointer overload instead.
- Python 2 support was removed in HepMC3 3.3.0.

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
  HepMC3 3.2.8.
