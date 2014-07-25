// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @author Andy Buckley
  class MC_PRINTEVENT : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    MC_PRINTEVENT()
      : Analysis("MC_PRINTEVENT")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    void init() {

      /// Set up particle name map
      // quarks & gluons
      _pnames[1] = "d";
      _pnames[-1] = "d~";
      _pnames[2] = "u";
      _pnames[-2] = "u~";
      _pnames[3] = "s";
      _pnames[-3] = "s~";
      _pnames[4] = "c";
      _pnames[-4] = "c~";
      _pnames[5] = "b";
      _pnames[-5] = "b~";
      _pnames[6] = "t";
      _pnames[-6] = "t~";
      // bosons
      _pnames[21] = "g";
      _pnames[22] = "gamma";
      _pnames[23] = "W+";
      _pnames[-23] = "W-";
      _pnames[-24] = "Z0";
      _pnames[-25] = "h0";
      _pnames[-26] = "h0";
      // leptons
      _pnames[11] = "e-";
      _pnames[-11] = "e+";
      _pnames[13] = "mu-";
      _pnames[-13] = "mu+";
      _pnames[15] = "tau-";
      _pnames[-15] = "tau+";
      _pnames[12] = "nu_e";
      _pnames[-12] = "nu_e~";
      _pnames[14] = "nu_mu";
      _pnames[-14] = "nu_mu~";
      _pnames[16] = "nu_tau";
      _pnames[-16] = "nu_tau~";
      // common hadrons
      _pnames[111] = "pi0";
      _pnames[211] = "pi+";
      _pnames[-211] = "pi-";
      _pnames[221] = "eta";
      _pnames[331] = "eta'";
      _pnames[113] = "rho0";
      _pnames[213] = "rho+";
      _pnames[-213] = "rho-";
      _pnames[223] = "omega";
      _pnames[333] = "phi";
      _pnames[130] = "K0L";
      _pnames[310] = "K0S";
      _pnames[311] = "K0";
      _pnames[-311] = "K0";
      _pnames[321] = "K+";
      _pnames[-321] = "K-";
      _pnames[313] = "K*0";
      _pnames[-313] = "K*0~";
      _pnames[323] = "K*+";
      _pnames[-323] = "K*-";
      _pnames[411] = "D+";
      _pnames[-411] = "D-";
      _pnames[421] = "D0";
      _pnames[-421] = "D0~";
      _pnames[413] = "D*+";
      _pnames[-413] = "D*-";
      _pnames[423] = "D*0";
      _pnames[-423] = "D*0~";
      _pnames[431] = "Ds+";
      _pnames[-431] = "Ds-";
      _pnames[433] = "Ds*+";
      _pnames[-433] = "Ds*-";
      _pnames[511] = "B0";
      _pnames[-511] = "B0~";
      _pnames[521] = "B+";
      _pnames[-521] = "B-";
      _pnames[513] = "B*0";
      _pnames[-513] = "B*0~";
      _pnames[523] = "B*+";
      _pnames[-523] = "B*-";
      _pnames[531] = "B0s";
      _pnames[541] = "Bc+";
      _pnames[-541] = "Bc-";
      _pnames[441] = "eta_c(1S)";
      _pnames[443] = "J/psi(1S)";
      _pnames[551] = "eta_b(1S)";
      _pnames[553] = "Upsilon(1S)";
      _pnames[2212] = "p+";
      _pnames[-2212] = "p-";
      _pnames[2112] = "n";
      _pnames[-2112] = "n~";
      _pnames[2224] = "Delta++";
      _pnames[2214] = "Delta+";
      _pnames[2114] = "Delta0";
      _pnames[1114] = "Delta-";
      _pnames[3122] = "Lambda";
      _pnames[-3122] = "Lambda~";
      _pnames[3222] = "Sigma+";
      _pnames[-3222] = "Sigma+~";
      _pnames[3212] = "Sigma0";
      _pnames[-3212] = "Sigma0~";
      _pnames[3112] = "Sigma-";
      _pnames[-3112] = "Sigma-~";
      _pnames[4122] = "Lambda_c+";
      _pnames[-4122] = "Lambda_c-";
      _pnames[5122] = "Lambda_b";
      // exotic
      _pnames[32] = "Z'";
      _pnames[34] = "W'+";
      _pnames[-34] = "W'-";
      _pnames[35] = "H0";
      _pnames[36] = "A0";
      _pnames[37] = "H+";
      _pnames[-37] = "H-";
      // shower-specific
      _pnames[91] = "cluster";
      _pnames[92] = "string";
      _pnames[9922212] = "remn";
      _pnames[1103] = "dd";
      _pnames[2101] = "ud0";
      _pnames[2103] = "ud1";
      _pnames[2203] = "uu";

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      /// @todo Wouldn't this be nice... if HepMC::IO_AsciiParticles was sane :-/
      // printEvent(event.genEvent());

      const GenEvent* evt = event.genEvent();

      cout << string(120, '=') << "\n" << endl;

      // Weights
      cout << "Weights(" << evt->weights().size() << ")=";
      for (vector<double>::const_iterator wgt = evt->weights().begin(); wgt != evt->weights().end(); ++wgt) {
        cout << *wgt << " ";
      }
/*      cout << "\n"
           << "EventScale " << evt->event_scale()
           << " [energy] \t alphaQCD=" << evt->alphaQCD()
           << "\t alphaQED=" << evt->alphaQED() << endl;
*/
      if (evt->pdf_info()) {
        evt->pdf_info()->print();
      } else {
        cout << "PdfInfo: EMPTY";
      }

      // Print a legend to describe the particle info
      char particle_legend[120];
      sprintf( particle_legend,"     %9s %8s %-15s %4s %8s %8s   (%9s,%9s,%9s,%9s,%9s)",
               "Barcode","PDG ID","Name","Stat","ProdVtx","DecayVtx","Px","Py","Pz","E ","m");
      cout << endl;
      cout << "                                       GenParticle Legend\n" << particle_legend << "\n";
      // if (m_vertexinfo) {
      //   sprintf( particle_legend," %60s (%9s,%9s,%9s,%9s)"," ","Vx","Vy","Vz","Vct ");
      //   cout << particle_legend << endl;
      // }
      // cout << string(120, '_') << endl;

      // Print all particles
      // const HepPDT::ParticleDataTable* pdt = m_ppsvc->PDT();
      for (HepMC3::GenEvent::particle_const_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p) {
        int p_bcode = (*p)->barcode();
        int p_pdg_id = (*p)->pdg_id();
        double p_px = (*p)->momentum().px();
        double p_py = (*p)->momentum().py();
        double p_pz = (*p)->momentum().pz();
        double p_pe = (*p)->momentum().e();

        int p_stat = (*p)->status();
        int p_prodvtx = 0;
        if ((*p)->production_vertex() && (*p)->production_vertex()->barcode() != 0) {
          p_prodvtx = (*p)->production_vertex()->barcode();
        }
        int p_endvtx = 0;
        if ((*p)->end_vertex() && (*p)->end_vertex()->barcode() != 0) {
          p_endvtx=(*p)->end_vertex()->barcode();
        }
        // double v_x = 0;
        // double v_y = 0;
        // double v_z = 0;
        // double v_ct = 0;
        // if ((*p)->production_vertex()) {
        //   v_x = (*p)->production_vertex()->position().x();
        //   v_y = (*p)->production_vertex()->position().y();
        //   v_z = (*p)->production_vertex()->position().z();
        //   v_ct = (*p)->production_vertex()->position().t();
        // }

        // Mass (prefer generated mass if available)
        double p_mass = (*p)->generated_mass();
        if (p_mass == 0 && !(p_stat == 1 && p_pdg_id == 22)) p_mass = (*p)->momentum().m();

        // Particle names
        string sname = (_pnames.find(p_pdg_id) != _pnames.end()) ? _pnames[p_pdg_id] : "";
        const char* p_name = sname.c_str() ;

        char particle_entries[120];
        sprintf(particle_entries, " %9i %8i %-15s %4i %8i %8i   (%+9.3g,%+9.3g,%+9.3g,%+9.3g,%9.3g)",
                p_bcode, p_pdg_id, p_name, p_stat, p_prodvtx, p_endvtx, p_px, p_py, p_pz, p_pe, p_mass);
        cout << particle_entries << "\n";
        // if (m_vertexinfo) {
        //   sprintf(particle_entries," %60s (%+9.3g,%+9.3g,%+9.3g,%+9.3g)"," ",v_x, v_y, v_z, v_ct);
        //   cout << particle_entries << "\n";
        // }
      }

      cout << "\n" << endl;
    }


    /// Normalise histograms etc., after the run
    void finalize() {    }

    //@}

  private:

    map<long, string> _pnames;


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_PRINTEVENT);

}
