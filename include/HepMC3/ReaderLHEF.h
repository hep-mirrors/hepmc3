// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"
#include <string>
#include <fstream>
#include <istream>
#include "HepMC3/LHEFAttributes.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include <iomanip>


namespace HepMC3 
{
class ReaderLHEF : public Reader
{
public:
    ReaderLHEF(){};
    ReaderLHEF(const std::string& filename);
    void initialize(const string &filename) override;
    bool read_event(GenEvent& ev) override;
    void close() override;
    bool failed() override;
    ~ReaderLHEF() ;
private:
    LHEF::Reader* m_reader;
    shared_ptr<HEPRUPAttribute> m_hepr;
    int m_neve;
    bool m_failed;
};
}




