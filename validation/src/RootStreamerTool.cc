// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#include "RootStreamerTool.h"
using namespace std;

int RootStreamerTool::process(GenEvent &hepmc) {
    HEPMC3CODE(
        IO_RootStreamer st;
        st.write_event(hepmc);
    )

    return 0;
}
