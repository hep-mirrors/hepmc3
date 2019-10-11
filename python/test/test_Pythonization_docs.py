import sys

v=sys.version_info
sys.path=["../"+str(v[0])+"."+str(v[1])+"."+str(v[2])+"/"]+sys.path

from  pyHepMC3TestUtils import COMPARE_ASCII_FILES
import pyHepMC3.pyHepMC3.HepMC3 as hm
import random

def test_Pythonization_docs():
    print (hm.GenEvent.particles.__doc__)
    print (hm.GenEvent.event_pos.__doc__)
    print (hm.GenVertex.particles_in.__doc__)
    return 0    

if __name__ == "__main__":
    result=1
    try:
     result=test_Pythonization_docs()
    except:
     result=1
    sys.exit(result)





