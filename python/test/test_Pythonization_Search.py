import sys

v=sys.version_info
sys.path=["../"+str(v[0])+"."+str(v[1])+"."+str(v[2])+"/"]+sys.path

from  pyHepMC3TestUtils import COMPARE_ASCII_FILES
import pyHepMC3.search.pyHepMC3search.HepMC3 as hmsearch
import pyHepMC3.pyHepMC3.HepMC3 as hm
import random,math


def test_Pythonization_Search():
    if sys.platform =='win32':
      return 0
    evt=hm.GenEvent(hm.Units.MomentumUnit.GEV,hm.Units.LengthUnit.CM)
    evt.set_event_number(1)
    evt.add_attribute("signal_process_id", hm.IntAttribute(20))
#     create vertex 1
    v1=hm.GenVertex()
    evt.add_vertex( v1 )
    p1=hm.GenParticle( hm.FourVector(1.0,1.0,7000,7000),2212, 3 )
    evt.add_particle( p1 )
    p1.add_attribute("flow1", hm.IntAttribute(231))
    p1.add_attribute("flow1", hm.IntAttribute(231))
    p1.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p1.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))

    v2=hm.GenVertex()
    evt.add_vertex( v2 )
    p2=hm.GenParticle( hm.FourVector(1.0,1.0,-7000,7000),2212, 3 )
    evt.add_particle( p2 )
    p2.add_attribute("flow1", hm.IntAttribute(243))
    p2.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p2.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))
    v2.add_particle_in( p2 )
#    
#     create the outgoing particles of v1 and v2
    p3=hm.GenParticle(hm.FourVector(.750,-1.569,32.191,32.238),1, 3 )
    evt.add_particle( p3 )
    p3.add_attribute("flow1", hm.IntAttribute(231))
    p3.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p3.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))
    v1.add_particle_out( p3 )
    p4=hm.GenParticle(hm.FourVector(-3.047,-19.,-54.629,57.920),-2, 3 )
    evt.add_particle( p4 )
    p4.add_attribute("flow1", hm.IntAttribute(243))
    p4.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p4.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))
    v2.add_particle_out( p4 )
#    
#     create v3
    v3=hm.GenVertex()
    evt.add_vertex( v3 )
    v3.add_particle_in( p3 )
    v3.add_particle_in( p4 )
    p6=hm.GenParticle( hm.FourVector(-3.813,0.113,-1.833,4.233 ),22, 1 )
    evt.add_particle( p6 )
    p6.add_attribute("flow1", hm.IntAttribute(231))
    p6.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p6.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))
    v3.add_particle_out( p6 )
    p5=hm.GenParticle(hm.FourVector(1.517,-20.68,-20.605,85.925),-24, 3 )
    evt.add_particle( p5 )
    p5.add_attribute("flow1", hm.IntAttribute(243))
    p5.add_attribute("theta", hm.DoubleAttribute(random.random()*math.pi))
    p5.add_attribute("phi", hm.DoubleAttribute(random.random()*math.pi*2))
    v3.add_particle_out( p5 )
#    
#     create v4
    v4=hm.GenVertex(hm.FourVector(0.12,-0.3,0.05,0.004))
    evt.add_vertex( v4 )
    v4.add_particle_in( p5 )
    p7=hm.GenParticle(hm.FourVector(-2.445,28.816,6.082,29.552), 1,1 )
    evt.add_particle( p7 )
    v4.add_particle_out( p7 )
    p8=hm.GenParticle(hm.FourVector(3.962,-49.498,-26.687,56.373), -2,1 )
    evt.add_particle( p8 )
    v4.add_particle_out( p8 )
    
    
    print(dir (hmsearch))
    print(hmsearch.applyFilter)
    print(hmsearch.Selector)
    print(dir(hmsearch.Selector))
    print(hmsearch.Selector.STATUS)
    #This is a bad code
    _MYSELECT1= lambda p: p.status() == 1
    _MYSELECT3= lambda p: p.status() == 3
    z1=filter(_MYSELECT1, evt.particles());
    z3=filter(_MYSELECT3, evt.particles());
    print (len(z1),len(z3))
    for a1 in z1:
      hm.Print.line(a1)
    z4=filter(lambda p: p.status() == 2, evt.particles());
    return 0    

if __name__ == "__main__":
    result=1
    result=test_Pythonization_Search()
    try:
     result=test_Pythonization_Search()
    except:
     result=1
    sys.exit(result)





