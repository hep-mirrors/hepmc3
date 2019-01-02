C...A simple skeleton program, illustrating a typical Pythia run:
C...Z0 production at LEP 1. 
C...Toy task: compare multiplicity distribution with matrix elements
C...and with parton showers (using same fragmentation parameters).
C...This code contains modifications for HepMC3 examples
C-----------------------------------------------------------------

C...Preamble: declarations.
 
C...All real arithmetic in double precision.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
C...Three Pythia functions return integers, so need declaring.
      INTEGER PYK,PYCHGE,PYCOMP

C...EXTERNAL statement links PYDATA on most machines.
      EXTERNAL PYDATA

C...Commonblocks.
C...The event record.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
C...Parameters.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
C...Decay information.
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
C...Selection of hard scattering subprocesses.
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
C...Parameters. 
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
C...Supersymmetry parameters.
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
C...Generation and cross section statistics.
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
C...HepMC3
      PARAMETER (NMXHEP=4000)
      COMMON /HEPEVT/  NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &                 JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),
     &                 VHEP(4,NMXHEP)
      INTEGER          NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      DOUBLE PRECISION PHEP,VHEP
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC      
C...These variables will be used for conversion. Block size is different
C...in Pythia6 and in HepMC3, so the most simple portable way is to have
C... a second block of same size as in HepMC3 and  copy the content of 
C...block directly.
      PARAMETER (NMXHEPL=10000)
      COMMON /HEPEVTL/  NEVHEPL,NHEPL,ISTHEPL(NMXHEPL),IDHEPL(NMXHEPL),
     &           JMOHEPL(2,NMXHEPL),JDAHEPL(2,NMXHEPL),PHEPL(5,NMXHEPL),
     &                 VHEPL(4,NMXHEPL)
      INTEGER          NEVHEPL,NHEPL,ISTHEPL,IDHEPL,JMOHEPL,JDAHEPL
      DOUBLE PRECISION PHEPL,VHEPL
      
      INTEGER OUTID(2), HEPMC3STATUS
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C...Glue functions
      INTEGER new_writer,delete_writer,set_hepevt_address
      INTEGER convert_event,write_event,clear_event
      INTEGER set_attribute_int,set_attribute_double
      EXTERNAL new_writer,delete_writer,set_hepevt_address
      EXTERNAL convert_event,write_event,clear_event
      EXTERNAL set_attribute_int,set_attribute_double
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C-----------------------------------------------------------------

C...First section: initialization.
 
C...Main parameters of run: c.m. energy and number of events.
      ECM=91.2D0
      NEV=1000

C...Select gamma*/Z0 production process.
      MSEL=0
      MSUB(1)=1

C...Only allow Z0 decay to quarks (i.e. no leptonic final states).
      DO 100 IDC=MDCY(23,2),MDCY(23,2)+MDCY(23,3)-1
        IF(IABS(KFDP(IDC,1)).GE.6) MDME(IDC,1)=MIN(0,MDME(IDC,1))
  100 CONTINUE

C...Initialize.
      CALL PYINIT('CMS','e+','e-',ECM)

C...Check that Z0 decay channels set correctly.
C      CALL PYSTAT(2)

C...Book histograms.
      CALL PYBOOK(1,'charged multiplicity ME',100,-0.5D0,99.5D0)
      CALL PYBOOK(2,'charged multiplicity PS',100,-0.5D0,99.5D0)
C...Create output writers
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      OUTID(1)=new_writer(0,1,'ME.hepmc')
      OUTID(2)=new_writer(0,1,'PS.hepmc')
      NEVHEP=-123456
      HEPMC3STATUS=set_hepevt_address(NEVHEPL)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC      
C-----------------------------------------------------------------

C...Second section: event loop.

C...Outer loop over ME and PS options.
      DO 300 ICA=1,2
        IF(ICA.EQ.1) THEN
          MSTP(48)=1
          MSTJ(101)=2
        ELSE
          MSTP(48)=0
        ENDIF 
 
 
C...Begin event loop.
        DO 200 IEV=1,NEV
          CALL PYEVNT

C...List first few events.
          IF(IEV.LE.2) CALL PYLIST(1)

C...Write output
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
          CALL PYHEPC(1)
          NEVHEPL=IEV
          NHEPL=NHEP
           DO 500 J=1,NHEP
          ISTHEPL(J)=ISTHEP(J)
          IDHEPL(J)=IDHEP(J)
          JMOHEPL(1,J)=JMOHEP(1,J)
          JMOHEPL(2,J)=JMOHEP(2,J)
          JDAHEPL(1,J)=JDAHEP(1,J)
          JDAHEPL(2,J)=JDAHEP(2,J)
          PHEPL(1,J)=PHEP(1,J)
          PHEPL(2,J)=PHEP(2,J)
          PHEPL(3,J)=PHEP(3,J)
          PHEPL(4,J)=PHEP(4,J)
          PHEPL(5,J)=PHEP(5,J)
          VHEPL(1,J)=VHEP(1,J)
          VHEPL(2,J)=VHEP(2,J)
          VHEPL(3,J)=VHEP(3,J)
          VHEPL(4,J)=VHEP(4,J)
  500     CONTINUE          
          HEPMC3STATUS=convert_event(OUTID(ICA))
C...Note: no XS uncertainty
          HEPMC3STATUS=set_cross_section(OUTID(ICA),
     &    1.0E9*XSEC(0,3),
     &    1.0E9*XSEC(0,3)/sqrt(1.0*NGEN(0,3)),
     &    NGEN(0,3),0)
          HEPMC3STATUS=set_attribute_int(OUTID(ICA),
     &    MSEL,'signal_process_id')
          HEPMC3STATUS=write_event(OUTID(ICA))
          HEPMC3STATUS=clear_event(OUTID(ICA))          
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 
C...Extract and fill event properties.
          CALL PYEDIT(3)
          CALL PYFILL(ICA,DBLE(N),1D0)
C...End event loop.
  200   CONTINUE

C...End outer loop.
  300 CONTINUE
C...Delete output writers
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 
      HEPMC3STATUS=delete_writer(OUTID(1))
      HEPMC3STATUS=delete_writer(OUTID(2))
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 
C-----------------------------------------------------------------

C...Third section: produce output and end.

C...Cross section table.
      CALL PYSTAT(1)

C...Histograms.
      CALL PYHIST

      END
