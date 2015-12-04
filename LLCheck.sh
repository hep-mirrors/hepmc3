#! /bin/bash -x

cd examples/LHEFExample
export LD_LIBRARY_PATH=/home/leif/MCEG/Projects/Work/HepMC3/outputs/lib:$LD_LIBRARY_PATH
../../outputs/bin/LHEF_example_cat.exe || exit 1
diff LHEF_example_out.lhe LHEF_example_cmp.lhe || exit 1
../../outputs/bin/HepMC3_fileIO_example.exe LHEF_example.hepmc3 copy.hepmc3 || exit 1
diff LHEF_example.hepmc3 copy.hepmc3 || exit 1
cd ../Pythia8Example
../../outputs/bin/pythia8_example.exe pythia8_ee_to_Z_to_tautau.conf output.hepmc3 || exit 1
../../outputs/bin/HepMC3_fileIO_example.exe output.hepmc3 copy.hepmc3 || exit 1
diff output.hepmc3 copy.hepmc3 || exit 1

