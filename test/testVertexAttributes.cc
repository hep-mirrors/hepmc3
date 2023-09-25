// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main() {
    auto r = std::make_shared<ReaderAscii>("inputVertexAttributes.hepmc");
    GenEvent e;
    r->read_event(e);
    r->close();
    auto v1 = e.vertices().back();
    auto barcode1 = v1->attribute<IntAttribute>("barcode");
    auto val1 = barcode1?barcode1->value():-10001;
    e.remove_vertex(e.vertices()[930]);
    auto v2 = e.vertices().back();
    auto barcode2 = v2->attribute<IntAttribute>("barcode");
    auto val2 = barcode2?barcode2->value():-10002;
    if (val1==val2) {
      return 0;
    }
    HepMC3::Print::line(v2,true);
    return 1;
}
