from .pyHepMC3 import *


def Pythonic_Reader_read(self):
   evt = HepMC3.GenEvent()
   self.read_event(evt)
   return evt

def Pythonic_Writer_write(self,evt):
   self.write_event(evt)


setattr(HepMC3.Writer, "write", Pythonic_Writer_write)
setattr(HepMC3.Reader, "read",  Pythonic_Reader_read)

setattr(HepMC3.GenEvent, "Xrun_info", property(lambda self: self.run_info(),lambda self,x: self.set_run_info(x)))
setattr(HepMC3.GenEvent, "Xevent_number", property(lambda self: self.event_number(),lambda self,x: self.set_event_number(x)))
setattr(HepMC3.GenEvent, "Xlength_unit", property(lambda self: self.length_unit(),lambda self,x: self.set_units(self.momentum_unit(),x)))
setattr(HepMC3.GenEvent, "Xmomentum_unit", property(lambda self: self.momentum_unit(),lambda self,x: self.set_units(x,self.length_unit())))
setattr(HepMC3.GenEvent, "Xparticles", property(lambda self: list(self.particles())))
setattr(HepMC3.GenEvent, "Xvertices", property(lambda self: list(self.vertices())))



setattr(HepMC3.GenRunInfo, "Xtools", property(lambda self: self.tools()))
setattr(HepMC3.GenRunInfo, "Xweight_names", property(lambda self: list(self.weight_names())))



setattr(HepMC3.Setup, "Xprint_warnings", property(lambda self: self.print_warnings,lambda self,x: self.set_print_warnings(x)))
setattr(HepMC3.Setup, "Xprint_errors", property(lambda self: self.print_errors,lambda self,x: self.set_print_errors(x)))
setattr(HepMC3.Setup, "Xdebug_level", property(lambda self: self.debug_level,lambda self,x: self.set_debug_level(x)))
