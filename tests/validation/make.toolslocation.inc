#
# Paths to validation tools
# Providing path to a tool will complie its interface and all tests
# that make use of this tool
#


# Photos++ path. Interface tested with Photospp v3.54
# wget http://photospp.web.cern.ch/photospp/resources/PHOTOS.3.54/PHOTOS.3.54.tar.gz
# ./configure --without-hepmc
PHOTOSLOCATION = ../../../Photospp.3.54/


# Tauola++ path. Interface tested with Tauola v1.1.4
# wget http://tauolapp.web.cern.ch/tauolapp/resources/TAUOLA.1.1.4/TAUOLA.1.1.4.tar.gz
# ./configure --without-hepmc
TAUOLALOCATION = ../../../Tauola.1.1.4/


# pythia8 path. Interface tested with pythia8.185
# wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8185.tgz
# ./configure --enable-shared
PYTHIA8LOCATION = ../../../pythia8185/


# MC-TESTER path. Interface tested with MC-TESTER v1.25.0
# wget http://mc-tester.web.cern.ch/MC-TESTER/MC-TESTER-1.25.0.tar.gz
MCTESTERLOCATION = ../../../mc-tester-1.25.0/
