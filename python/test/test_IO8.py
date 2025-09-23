from pyHepMC3TestUtils import update_path, python_label
import sys
import time

sys.path = update_path()


from pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3 import HepMC3 as hm
from pyHepMC3.rootIO import HepMC3 as hmrootIO

print(dir(hmrootIO))


def test_IO81(ext,form):
    inputA = hm.ReaderAsciiHepMC2("inputIO8.hepmc")
    if inputA.failed():
        sys.exit(1)
    outputA = hm.WriterGZ("WriterAscii",python_label() + "frominputIO8.hepmc"+ext,form)
    if outputA.failed():
        sys.exit(12)
    while not inputA.failed():
        evt = hm.GenEvent()
        inputA.read_event(evt)
        if inputA.failed():
            print("End of file reached. Exit.\n")
            break
        outputA.write_event(evt)
        evt.clear()
    inputA.close()
    outputA.close()


def test_IO82(ext,form):
    inputB = hm.ReaderGZ("ReaderAscii",python_label() + "frominputIO8.hepmc"+ext,form)
    if inputB.failed():
        sys.exit(3)
    outputB = hm.WriterAsciiHepMC2(python_label() + "fromfrominputIO8"+ext+".hepmc")
    if outputB.failed():
        sys.exit(4)
    while not inputB.failed():
        evt = hm.GenEvent()
        inputB.read_event(evt)
        print(evt.event_number())
        if inputB.failed():
            print("End of file reached. Exit.\n")
            break
        outputB.write_event(evt)
        evt.clear()
    inputB.close()
    outputB.close()
    assert 0 == COMPARE_ASCII_FILES(python_label() + "fromfrominputIO8"+ext+".hepmc", "inputIO8.hepmc")
    return 0



if __name__ == "__main__":
    result = 1
    try:
        test_IO81(".gz","gzip")
#        test_IO81(".lzma","lzma")
#        test_IO81(".z","zstandard")
#        test_IO81(".bz2","bz2")
        result = 0
        result += test_IO82(".gz","gzip")
#        result += test_IO82(".lzma","lzma")
#        result += test_IO82(".z","zstandard")
#        result += test_IO82(".bz2","bz2")
    except:
        result = 1
    sys.exit(result)
