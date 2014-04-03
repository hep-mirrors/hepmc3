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
# MC-TESTER path. Interface tested with MC-TESTER v1.25.0
# wget http://mc-tester.web.cern.ch/MC-TESTER/MC-TESTER-1.25.0.tar.gz
MCTESTERLOCATION = ../../../mc-tester-1.25.0/
###############################################################################



PHOTOSINTERFACE = Photospp
PHOTOSINC       = -I$(PHOTOSLOCATION)/include \
                  -I$(PHOTOSINTERFACE)/include
PHOTOSLIBS      = -L$(PHOTOSLOCATION)/lib  -lPhotosCxxInterface -lPhotosFortran \
                  -L$(PHOTOSINTERFACE)/lib -lPhotosHepMC3Interface

MCTESTERINTERFACE = mc-tester
MCTESTERINC       = -I$(MCTESTERLOCATION)/include \
                    -I$(MCTESTERINTERFACE)/include
MCTESTERLIBS      = -L$(MCTESTERLOCATION)/lib  -lHEPEvent -lMCTester \
                    -L$(MCTESTERINTERFACE)/lib -lHepMC3Interface

