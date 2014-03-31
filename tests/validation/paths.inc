# Photos++ path. Interface tested with Photospp v3.54
# wget http://photospp.web.cern.ch/photospp/resources/PHOTOS.3.54/PHOTOS.3.54.tar.gz
# ./configure --without-hepmc
PHOTOSLOCATION = ../../../Photospp.3.54/

PHOTOSINTERFACE = Photospp
PHOTOSINC       = -I$(PHOTOSLOCATION)/include \
                  -I$(PHOTOSINTERFACE)/include
PHOTOSLIBS      = -L$(PHOTOSLOCATION)/lib  -lPhotosCxxInterface -lPhotosFortran \
                  -L$(PHOTOSINTERFACE)/lib -lPhotosHepMC3Interface


