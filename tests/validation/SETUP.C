{
Setup::decay_particle=-15;
Setup::mass_power=1;
Setup::mass_scale_on=true;

// Setup histograms
int n_bins=60;
double default_min_bin=0.0;
double default_max_bin=1.1;
Setup::SetHistogramDefaults(n_bins,default_min_bin,default_max_bin);

// Setup User Histograms
//Setup::UserTreeAnalysis = "RhoRhoUserTreeAnalysis";

// Description
Setup::result1_path = "hepmc2.root";
Setup::gen1_desc_1="HepMC3 validation";
Setup::gen1_desc_2="Z decays";
Setup::gen1_desc_3=" HepMC2 sample ";

Setup::result2_path = "hepmc3.root";
Setup::gen1_desc_1="HepMC3 validation";
Setup::gen1_desc_2="Z decays";
Setup::gen1_desc_3=" HepMC3 sample ";

if (Setup::stage==0)
    printf("Setup loaded from SETUP.C, ANALYSIS stage.\n");
else
    printf("Setup loaded from SETUP.C, GENERATION stage %i.\n",Setup::stage);

Setup::SuppressDecay(111); // suppress pi0 decays

};

