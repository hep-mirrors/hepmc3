## Introduction
The HepMC package is an object oriented, C++ event record for High Energy Physics Monte Carlo generators and simulation.
    
It is described in [A. Buckley et al.,The HepMC3 Event Record Library for Monte Carlo Event Generators (Comput.Phys.Commun. 260 (2021) 107310)](https://doi.org/10.1016/j.cpc.2020.107310).

The predecessor library is described in [M. Dobbs and J.B. Hansen,
    The HepMC C++ Monte Carlo event record for High Energy Physics (Comput. Phys. Commun. 134 (2001) 41)](https://doi.org/10.1016/S0010-4655(00)00189-2)
    
Since the first implementation, the package has undergone several modifications and in particular,
    the latest HepMC3 series is a completely new re-write using currently available C++11 techniques,
    and have out-of-the-box interfaces for the widely used in HEP community ROOT and Python.

Further documentation that might be useful for the users of HepMC3 can be found in the following documents:
   - [S. Gardiner, J. Isaacson and L. Pickering, NuHepMC: A standardized event record format for neutrino event generators](https://arxiv.org/abs/2310.13211)
   - [E. Bothmann et al., A standard convention for particle-level Monte Carlo event-variation weights(SciPost Phys.Core 6 (2023) 007)](https://doi.org/10.21468/SciPostPhysCore.6.1.007)</a>
   - [J. Alwall et al., A standard format for Les Houches Event Files (Comput.Phys.Commun. 176 (2007) 300-304)](https://doi.org/10.1016/j.cpc.2006.11.010)
   - [T. Sjöstrand et al., A proposed standard event record, in `Z physics at LEP 1', eds. G. Altarelli, R. Kleiss and C. Verzegnassi, Geneva, Switzerland, September 4-5, 1989, CERN 89-08 (Geneva, 1989), Vol. 3, p. 327](https://inis.iaea.org/collection/NCLCollectionStore/_Public/21/009/21009075.pdf)
   - [E. Boos et al., Generic User Process Interface for Event Generators](http://arxiv.org/abs/hep-ph/0109068)
   - [J. Butterworth et al., THE TOOLS AND MONTE CARLO WORKING GROUP Summary Report from the Les Houches 2009 Workshop on TeV Colliders](http://arxiv.org/abs/1003.1643) 
   - [J. R. Andersen et al., Les Houches 2013: Physics at TeV Colliders: Standard Model Working Group Report](http://arxiv.org/abs/1405.1067)


## LHEF-HDF5 I/O

When built with `HEPMC3_ENABLE_HDF5IO=ON`, HepMC3 provides readers and
writers for Les Houches Event data stored in HDF5 files. The
`HepMC3/LHEFHDF5.h` header defines `LHEF::ReaderHDF5` and
`LHEF::WriterHDF5`, which expose the familiar `LHEF::HEPRUP` and
`LHEF::HEPEUP` run and event objects. `HepMC3/ReaderLHEFHDF5.h` provides
`HepMC3::ReaderLHEFHDF5` to convert LHEF-HDF5 input directly to
`HepMC3::GenEvent` objects.

Several LHEF-HDF5 layouts are in use. SHERPA-MC HDF5 event output and
Pythia8 `LHAHDF5v2` use the flat dataset layout implemented by
`LHEFHDF5` and are compatible with its reader and writer. Older Pythia8
`LHAHDF5` formats, including version 1.0.0, use grouped column datasets and
are not compatible with the flat layout. Users must select the reader and
writer appropriate to the HDF5 format produced by their generator.

The HDF5-based LHEH5 format is described in [A. Puck Neuwirth, M. Feickert,
L. Heinrich, and E. Rodrigues, *pylhe: A Lightweight Python interface to Les
Houches Event files*](https://arxiv.org/abs/2607.29352). The PEPPER-MC
implementation and its scalable HDF5 event I/O are described in
[E. Bothmann et al., *Efficient precision simulation of processes with
many-jet final states at the LHC*, Phys. Rev. D 109 (2024) 014013](https://doi.org/10.1103/PhysRevD.109.014013).

## Project documentation, code and contacts

Project documentation, code and contacts
   -  [One page documentation](full.md)
   -  [HepMC3 CERN GitLab repository](https://gitlab.cern.ch/hepmc/HepMC3)
   -  [hepmc-dev@NOSPAM.cern.ch mailing list](mailto:hepmc-dev@cern.ch)
   -  [CERN GitLab issues tracker](https://gitlab.cern.ch/hepmc/HepMC3/-/issues)

## Available tarfiles

Development version (updated daily at 3AM): [hepmc3-dev.tgz](releases/hepmc3-dev.tgz)

Releases
   -  [HepMC3-3.2.7.tar.gz, Dec.2023](releases/HepMC3-3.2.7.tar.gz)
   -  [HepMC3-3.2.6.tar.gz, Mar.2023](releases/HepMC3-3.2.6.tar.gz)
   -  [HepMC3-3.2.5.tar.gz, 22.Feb.2022](releases/HepMC3-3.2.5.tar.gz)
   -  [HepMC3-3.2.4.tar.gz, 07.Jul.2021](releases/HepMC3-3.2.4.tar.gz)
   -  [HepMC3-3.2.3.tar.gz, 14.Dec.2020](releases/HepMC3-3.2.3.tar.gz)
   -  [HepMC3-3.2.2.tar.gz, 10.Jun.2020](releases/HepMC3-3.2.2.tar.gz)
   -  [HepMC3-3.2.1.tar.gz, 17.Mar.2020](releases/HepMC3-3.2.1.tar.gz)
   -  [HepMC3-3.2.0.tar.gz, 27.Nov.2019](releases/HepMC3-3.2.0.tar.gz)
   -  [HepMC3-3.1.2.tar.gz, 23.Aug.2019](releases/HepMC3-3.1.2.tar.gz)

## Related projects
    
### HepMC2
The HepMC2 series development has been essentially stopped and in particular
    HepMC2.06.x (with the latest being 2.06.11) versions are frozen. 

   - [HepMC2 CERN GitLab repository](https://gitlab.cern.ch/hepmc/HepMC)    
    
   - HepMC2 tarballs
      - [hepmc2.06.11.tgz, 10.June.2020](releases/hepmc2.06.11.tgz)
      - [hepmc2.06.10.tgz, 26.July.2019](releases/hepmc2.06.10.tgz)
      - [hepmc2.07-beta00.tgz, 19.August.2016](releases/hepmc2.07-beta00.tgz)
    
   - Documentation
      - [HepMC2.06  user manual](releases/HepMC2_user_manual.pdf)
      - [HepMC2.06 reference manual](releases/HepMC2_reference_manual.pdf)

### HepPDT
HepPDT tarballs
   - [HepPDT-3.04.01.tar.gz](releases/HepPDT-3.04.01.tar.gz)

