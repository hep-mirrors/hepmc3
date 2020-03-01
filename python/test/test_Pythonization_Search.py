from  pyHepMC3TestUtils import update_path
import sys,os
sys.path=update_path()

from  pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3.search import HepMC3 as hmsearch
import random,math


def test_Pythonization_Search():
    print (dir(hmsearch))
    return 0

if __name__ == "__main__":
    result=1
    try:
     result=test_Pythonization_Search()
    except:
     result=1
    sys.exit(result)





