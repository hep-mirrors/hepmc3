from .pyHepMC3 import *


def Pythonic_Reader_read(self):
   evt = HepMC3.GenEvent()
   self.read_event(evt)
   return evt

def Pythonic_Writer_write(self,evt):
   self.write_event(evt)


setattr(HepMC3.Writer, "py_write", Pythonic_Writer_write)
setattr(HepMC3.Reader, "py_read",  Pythonic_Reader_read)

#setattr(HepMC3.GenEvent, "py_weights", property(lambda self: list(self.weights()),lambda self,x: self.set_weights(x)))
setattr(HepMC3.GenEvent, "py_run_info", property(lambda self: self.run_info(),lambda self,x: self.set_run_info(x)))
setattr(HepMC3.GenEvent, "py_event_number", property(lambda self: self.event_number(),lambda self,x: self.set_event_number(x)))
setattr(HepMC3.GenEvent, "py_length_unit", property(lambda self: self.length_unit(),lambda self,x: self.set_units(self.momentum_unit(),x)))
setattr(HepMC3.GenEvent, "py_momentum_unit", property(lambda self: self.momentum_unit(),lambda self,x: self.set_units(x,self.length_unit())))
setattr(HepMC3.GenEvent, "py_particles", property(lambda self: list(self.particles())))
setattr(HepMC3.GenEvent, "py_vertices", property(lambda self: list(self.vertices())))



setattr(HepMC3.GenRunInfo, "py_tools", property(lambda self: self.tools()))
setattr(HepMC3.GenRunInfo, "py_weight_names", property(lambda self: list(self.weight_names())))


setattr(HepMC3.GenParticle, "py_generated_mass", property(lambda self: self.generated_mass(),lambda self,x: self.set_generated_mass(x)))
setattr(HepMC3.GenParticle, "py_pid", property(lambda self: self.pid(),lambda self,x: self.set_pid(x)))
setattr(HepMC3.GenParticle, "py_status", property(lambda self: self.status(),lambda self,x: self.set_status(x)))
setattr(HepMC3.GenParticle, "py_momentum", property(lambda self: self.momentum(),lambda self,x: self.set_momentum(x)))
setattr(HepMC3.GenParticle, "py_parents", property(lambda self: list(self.production_vertex().particles_in())))

#setattr(HepMC3.GenHeavyIon, "py_status", property(lambda self: self.Ncoll_hard(),lambda self,x: self.set_status(x)))


setattr(HepMC3.Setup, "py_print_warnings", property(lambda self: self.print_warnings,lambda self,x: self.set_print_warnings(x)))
setattr(HepMC3.Setup, "py_print_errors", property(lambda self: self.print_errors,lambda self,x: self.set_print_errors(x)))
setattr(HepMC3.Setup, "py_debug_level", property(lambda self: self.debug_level,lambda self,x: self.set_debug_level(x)))
