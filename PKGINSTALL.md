# Installing pre-built HepMC3 packages

This page documents availability of pre-built HepMC3 packages from
various sources, for various OS platforms.

Note that the available HepMC3 version(s) depend on the distribution
or repository and may not include the latest version.


## LCG platforms

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

HepMC3 is available from the standard package-repositories of multiple
Linux distributions, currently Fedora, CentOS/EPEL, openSUSE, Arch,
Gentoo. (Note, the Debian/Ubuntu package is outdated: prefer another
source.)

This approach usually requires system-administrator permissions. With
appropriate permissions, the following commands will install HepMC3 in
each system:

### Fedora
HepMC3 is available from the standard repository. To install:

```sh
sudo dnf install HepMC3 HepMC3-devel HepMC3-search HepMC3-search-devel HepMC3-interfaces-devel HepMC3-doc
```

To have a full installation of the HepMC3-doc package, add the option `--setopt=tsflags=''`

### RHEL and compatible
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
https://aur.archlinux.org/packages/hepmc .  To
install:

```sh
sudo pacman -Syu hepmc
```

The dependencies can vary.

### Gentoo
HepMC3 is available in the standard repository
https://packages.gentoo.org/packages/sci-physics/hepmc.
To install:

```sh
sudo emerge --ask hepmc:3
```

## MacOSX

HepMC3 is available in the ``homebrew-hep`` repository
https://davidchall.github.io/homebrew-hep/.  To install:

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
