from  pyHepMC3TestUtils import update_path
import sys,os
sys.path=update_path()

 
from  pyHepMC3TestUtils import COMPARE_ASCII_FILES
import pyHepMC3.pyHepMC3.HepMC3 as hm
import pyHepMC3.rootIO.pyHepMC3rootIO.HepMC3 as hmrootIO
print(dir(hmrootIO))
def test_IO2():
 inputA=hm.ReaderAsciiHepMC2("inputIO2.hepmc")
 if inputA.failed(): sys.exit(1)
 outputA=hmrootIO.WriterRootTree  ("frominputIO2.root")
 if outputA.failed(): sys.exit(2)
 while  not inputA.failed():
        evt=hm.GenEvent()
        inputA.read_event(evt)
        if inputA.failed(): 
            print("End of file reached. Exit.\n")
            break
        outputA.write_event(evt)
        evt.clear()    
 inputA.close()
 outputA.close()
 
 inputB=hmrootIO.ReaderRootTree ("frominputIO2.root")
 if inputB.failed(): sys.exit(3)
 outputB=hm.WriterAsciiHepMC2("fromfrominputIO2.hepmc")
 if outputB.failed(): sys.exit(4)
 while not inputB.failed():
        evt=hm.GenEvent()
        inputB.read_event(evt)
        if  inputB.failed():  
            print ("End of file reached. Exit.\n")
            break
        outputB.write_event(evt)
        evt.clear()    
 inputB.close()
 outputB.close()
 assert(0 == COMPARE_ASCII_FILES("fromfrominputIO2.hepmc","inputIO2.hepmc"))
 return 0
if __name__ == "__main__":
    result=1
    try:
     result=test_IO2()
    except:
     result=1
    sys.exit(result)
