from pyHepMC3TestUtils import update_path, python_label
import sys


sys.path = update_path()

from pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3 import HepMC3 as hm


def test_IO4():
    if sys.version_info[0]<3:
      return 0
    if sys.implementation.name != 'cpython':
      return 0
    exts = ["gzip", "bz2", "lzma"]
    import lzma
    import gzip
    import bz2
    import shutil
    zstdavail = True
    try:
      import zstandard
      if not hasattr(zstandard,'open'):
        zstdavail = False  
    except ImportError as e:
      zstdavail = False

    with open('inputIO4.hepmc', 'rb') as f_in:
      with gzip.open("inputIO4.hepmc.gzip", 'wb') as f_out:
        shutil.copyfileobj(f_in, f_out)

    with open('inputIO4.hepmc', 'rb') as f_in:
      with bz2.open("inputIO4.hepmc.bz2", 'wb') as f_out:
        shutil.copyfileobj(f_in, f_out)

    with open('inputIO4.hepmc', 'rb') as f_in:
      with lzma.open("inputIO4.hepmc.lzma", 'wb') as f_out:
        shutil.copyfileobj(f_in, f_out)
    if zstdavail:
      exts = ["gzip", "bz2", "lzma", "zstd"]
      with open('inputIO4.hepmc', 'rb') as f_in:
        with zstandard.open("inputIO4.hepmc.zstd", 'wb') as f_out:
          shutil.copyfileobj(f_in, f_out)

    for ext in exts:
      inputA = hm.deduce_reader("inputIO4.hepmc."+ext)
      if inputA.failed():
          sys.exit(1)
      outputA = hm.WriterAsciiHepMC2("inputIO4.hepmcfrom"+ext)
      if outputA.failed():
          sys.exit(2)
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
      assert 0 == COMPARE_ASCII_FILES("inputIO4.hepmcfrom"+ext, "inputIO4.hepmc")
    return 0


if __name__ == "__main__":
    result = 1
    try:
        result = test_IO4()
    except:
        result = 1
    sys.exit(result)
