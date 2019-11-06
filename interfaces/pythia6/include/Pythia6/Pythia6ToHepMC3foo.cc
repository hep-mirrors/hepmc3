// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef Pythia6_Pythia6ToHepMC3foo_H
#define Pythia6_Pythia6ToHepMC3foo_H

extern "C" {

    int hepmc3_delete_writer_(const int & position)
    {
        return 0;
    }
    int hepmc3_convert_event_(const int & position)
    {
        return 0;
    }
    int hepmc3_write_event_(const int & position)
    {
        return 0;
    }
    int hepmc3_clear_event_(const int & position)
    {
        return 0;
    }
    int hepmc3_set_cross_section_(const int & position, const double& x,const double& xe, const int& n1,const int& n2)
    {
        return 0;
    }

    int hepmc3_set_pdf_info_(const int & position,const int& parton_id1, const int& parton_id2, const double& x1, const double& x2,
                      const double& scale_in, const double& xf1,const double& xf2,
                      const int& pdf_id1, const int& pdf_id2)
    {
        return 0;
    }
    int hepmc3_set_hepevt_address_(int* a)
    {
            return 0;
    }
    int hepmc3_set_attribute_int_(const int & position,const int & attval,const char* attname)
    {
        return 0;
    }
    int hepmc3_set_attribute_double_(const int & position,const double & attval,const char* attname)
    {
        return 0;
    }
    int hepmc3_new_writer_(const int & position,const int & mode,const char* ffilename)
    {
        return  0;
    }
    int hepmc3_new_weight_(const int & position, const char* name)
    {
        return 0;
    }
    int hepmc3_set_weight_by_index_(const int & position,const double& val, const int & pos)
    {
        return 0;
    }
    int hepmc3_set_weight_by_name_(const int & position,const double& val, const char* name)
    {
        return 0;
    }  
}
#endif
