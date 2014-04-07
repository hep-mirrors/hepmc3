###############################################################################
#
# Paths to validation tools
# Remove path to only compile tests/libraries that do not require these tools
#
# Photos++ path. Interface tested with Photospp v3.54
# wget http://photospp.web.cern.ch/photospp/resources/PHOTOS.3.54/PHOTOS.3.54.tar.gz
# ./configure --without-hepmc
PHOTOSLOCATION = ../../../Photospp.3.54/
#
# Tauola++ path. Interface tested with Tauola v1.1.4
# wget http://tauolapp.web.cern.ch/tauolapp/resources/TAUOLA.1.1.4/TAUOLA.1.1.4.tar.gz
# ./configure --without-hepmc
TAUOLALOCATION = ../../../Tauola.1.1.4/
#
# MC-TESTER path. Interface tested with MC-TESTER v1.25.0
# wget http://mc-tester.web.cern.ch/MC-TESTER/MC-TESTER-1.25.0.tar.gz
MCTESTERLOCATION = ../../../mc-tester-1.25.0/
###############################################################################



PHOTOSINTERFACE = Photospp
PHOTOSINC       = -I$(PHOTOSLOCATION)/include \
                  -I$(PHOTOSINTERFACE)/include
PHOTOSLIBS      = -L$(PHOTOSLOCATION)/lib  -lPhotosCxxInterface -lPhotosFortran \
                  -L$(PHOTOSINTERFACE)/lib -lPhotosHepMC3Interface

TAUOLAINTERFACE = Tauolapp
TAUOLAINC       = -I$(TAUOLALOCATION)/include \
                  -I$(TAUOLAINTERFACE)/include
TAUOLALIBS      = -L$(TAUOLALOCATION)/lib  -lTauolaCxxInterface -lTauolaFortran \
                  -L$(TAUOLAINTERFACE)/lib -lTauolaHepMC3Interface

MCTESTERINTERFACE = mc-tester
MCTESTERINC       = -I$(MCTESTERLOCATION)/include \
                    -I$(MCTESTERINTERFACE)/include \
                    -I$(shell root-config --incdir)
MCTESTERLIBS      = -L$(MCTESTERLOCATION)/lib  -lHEPEvent -lMCTester \
                    -L$(MCTESTERINTERFACE)/lib -lHepMC3Interface \
                    $(shell root-config --glibs)
