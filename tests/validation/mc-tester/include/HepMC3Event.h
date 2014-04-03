#ifndef _HepMC3Event_H
#define _HepMC3Event_H

#include "HepMC3/GenEvent.h"
#include "HepMC3Particle.h"
#include "HEPEvent.H"

class HepMC3Event: public HEPEvent
{
private:
  /** List of particles in the event */
  HepMC3Particle **particles;

  /** Flag for how particles decaying into there own type are treated */
  bool count_self_decays;
 
public:
  /** Constructor for HepMC3Event. Creates a new event using the
      event info from HepMC3::GenEvent e. Also copies each particle
      into a HepMC3Particle and stores them as a list. */
  HepMC3Event(HepMC3::GenEvent &e, bool include_self_decay=true);
  /** Destructor for HepMC3Event */
  ~HepMC3Event();

  /** return the number of particles in the event */
  int GetNumOfParticles();

  /** returns the event number */
  int GetEventNumber();
  
  /** Dummy function definition. Do not use */
  void SetNumOfParticles(int n);

  /** Dummy function definition. Do not use */
  void SetEventNumber(int ev);
  
  /** Returns the HEPParticle with id "idx". This is the id number as used
   by MC-TESTER and not the id number from the original HepMC3::GenParticle.
   Note: Indecies begin at 1.*/
  HEPParticle* GetParticle( int idx);

  /** Returns the HepMC3Particle with id "barcode". This is the barcode 
    number from the original HepMC3::GenParticle and not the ID used by 
    MC-TESTER. This function is not needed in HEPEvent, but is needed
    by HepMC3Particle::Get_Daughters(). */
  HepMC3Particle* GetParticleWithBarcode( int barcode );
  
  /** Dummy function definition. Do not use */
  void  SetParticle( int idx, HEPParticle *particle) ;

  /** Dummy function definition. Do not use */
  void  AddParticle( HEPParticle *particle);

  /** Dummy function definition. Do not use */
  void  Clear (int fromIdx);
  
  /** Dummy function definition. Do not use */
  void  InsertParticle(int at_idx,HEPParticle *p);
  
  /** Dummy function definition. Do not use */
  void  AddParticle( int id, int pdgid, int status,
			     int mother, int mother2,
			     int firstdaughter, int lastdaughter,
			     double E,double px, double py, double pz, double m,
			     double vx, double vy, double vz, double tau);

  std::vector<double> * Sum4Momentum();

  bool CountSelfDecays(){ return count_self_decays; };

  /** Implementation of FindParticle needed for excluding "self decays" */
  HEPParticleList* FindParticle(int pdg, HEPParticleList *list);

private:
  HepMC3::GenEvent *evt;
  int m_particle_count;
};

#endif
