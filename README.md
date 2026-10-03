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


## Installing pre-built HepMC3 packages

HepMC3 is available through several pre-built software distributions
and package managers, including the [LHC Computing Grid
(LCG)](https://home.cern/science/computing/grid/),
[Conda](https://docs.conda.io/projects/conda/en/latest/index.html),
several Linux-distribution package repositories, and installers for
Mac OS X and Windows. For full instructions, see [the dedicated
package-installation page](PKGINSTALL.md).


## Installing HepMC3 from source

If using pre-built HepMC3 installation from the repositories is not
possible or is not desired, you can build HepMC3 from the source. For
full instructions, see [the dedicated source-installation
page](SRCINSTALL.md).


## Installation troubleshooting

Problems during the HepMC3 installation can potentially be caused by

 - A C++ compiler that does not support C++11.
   The only solution is to use compiler with C++11 support
 - The CMake version is too old.
 - Compilation of Python bindings fails. `pybind11`, which is used by
   HepMC3 to create the Python bindings supports only some compilers
   (`gcc`, `clang`, MSVC), therefore it is expected that the bindings
   will only work with these compilers.


##  Usage examples

HepMC3 is shipped with multiple example programs, useful both to
exemplify the API and possibly direct solutions to some event-handling
tasks. For information on building and using the example programs, see
[the dedicated examples page](EXAMPLES.md).


## Compatibility and deprecation notes

This section documents I/O-format and code compatibility issues for
HepMC users to be aware of:

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
