// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Test that we can correctly read ASCII events with additional
// whitespace
//

// These are the only headers in ReaderPlugin, so including these firstmakes
// sure the hack below doesn't leak out of the ReaderPlugin header
#include "HepMC3/Print.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderFactory.h"
#include "HepMC3/Setup.h"

std::vector<HepMC3::ConstGenParticlePtr> get_parts_with(HepMC3::GenEvent const &ev, int status, int pid) {
  std::vector<HepMC3::ConstGenParticlePtr> out_parts;

  for (auto const &p : ev.particles()) {
    bool sel = !status || (p->status() == status);
    sel &= !pid || (p->pid() == pid);
    if (sel)
      out_parts.push_back(p);
  }

  if (!out_parts.size()) {
    std::stringstream ss;
    ss << "failed to find any parts with status = " << status
       << ", and pid = " << pid << " in event.";
    throw std::runtime_error(ss.str());
  }

  return out_parts;
}

int main() {

  HepMC3::Setup::set_print_errors(true);
  HepMC3::Setup::set_errors_level(999);
  HepMC3::Setup::set_print_warnings(true);
  HepMC3::Setup::set_warnings_level(999);
  HepMC3::Setup::set_debug_level(999);

  auto rdr = HepMC3::deduce_reader("inputIO30.hepmc");
  if (!rdr)
    return 1;

  HepMC3::GenEvent ev;

  /* Event with normal whitespace */
  std::cout << "reading event with normal whitespace..." << std::endl;
  rdr->read_event(ev);
  std::stringstream ev1_list;
  HepMC3::Print::listing(ev1_list, ev);
  HepMC3::Print::listing(ev);

  if (rdr->failed()) {
    std::cout << "-- Failed reading event" << std::endl;
    return 1;
  }

  auto attr1 = rdr->run_info()->attribute<HepMC3::IntAttribute>("Attr1");
  if (attr1->value() != 123) {
    std::cout << "-- Failed parsing attribute Attr1 correctly: expected 123 "
                 "but found "
              << attr1->value() << std::endl;
    return 1;
  }
  auto attr2 = rdr->run_info()->attribute<HepMC3::IntAttribute>("Attr2");
  if (attr2->value() != 321) {
    std::cout << "-- Failed parsing attribute Attr2 correctly: expected 321 "
                 "but found "
              << attr2->value() << std::endl;
    return 1;
  }
  auto attr3 = rdr->run_info()->attribute<HepMC3::IntAttribute>("Attr3");
  if (attr3->value() != 456) {
    std::cout << "-- Failed parsing attribute Attr3 correctly: expected 456 "
                 "but found "
              << attr3->value() << std::endl;
    return 1;
  }

  {
    if (ev.particles().size() != 8) {
      std::cout << "-- Expected to find 8 particles in the event, but found "
                << ev.particles().size() << std::endl;
      return 1;
    }
    if (ev.vertices().size() != 2) {
      std::cout << "-- Expected to find 2 vertices in the event, but found "
                << ev.particles().size() << std::endl;
      return 1;
    }

    auto parts_s4_p14 = get_parts_with(ev, 4, 14);
    if (parts_s4_p14.size() != 1) {
      std::cout << "-- Expected to find 1 particle with status 4 and pid 14, "
                   "but found "
                << parts_s4_p14.size() << std::endl;
      return 1;
    }
    if (parts_s4_p14.front()->momentum().x() != 1.23) {
      std::cout
          << "-- Expected to find x momentum of particle with status 4 and "
             "pid 14 = 1.23, but found "
          << parts_s4_p14.front()->momentum().x() << std::endl;
      return 1;
    }

    auto parts_s21_p2212 = get_parts_with(ev, 21, 2212);
    if (parts_s21_p2212.size() != 1) {
      std::cout
          << "-- Expected to find 1 particle with status 21 and pid 2212, "
             "but found "
          << parts_s21_p2212.size() << std::endl;
      return 1;
    }
    if (parts_s21_p2212.front()->momentum().z() != 1.23) {
      std::cout
          << "-- Expected to find z momentum of particle with status 21 and "
             "pid 2212 = 1.23, but found "
          << parts_s21_p2212.front()->momentum().z() << std::endl;
      return 1;
    }

    auto parts_s1_p211 = get_parts_with(ev, 1, 211);
    if (parts_s1_p211.size() != 1) {
      std::cout << "-- Expected to find 1 particle with status 1 and pid 211, "
                   "but found "
                << parts_s1_p211.size() << std::endl;
      return 1;
    }
    if (parts_s1_p211.front()->generated_mass() != 1.23) {
      std::cout
          << "-- Expected to find the gen mass of particle with status 1 and "
             "pid 211 = 1.23, but found "
          << parts_s21_p2212.front()->generated_mass() << std::endl;
      return 1;
    }
  }

  /* Event with extra whitespace */
  std::cout << "reading event with extra whitespace..." << std::endl;
  rdr->read_event(ev);
  std::stringstream ev2_list;
  HepMC3::Print::listing(ev2_list, ev);
  HepMC3::Print::listing(ev);

  if (rdr->failed()) {
    std::cout << "-- Failed reading event" << std::endl;
    return 1;
  }

  {
    if (ev.particles().size() != 8) {
      std::cout << "-- Expected to find 8 particles in the event, but found "
                << ev.particles().size() << std::endl;
      return 1;
    }
    if (ev.vertices().size() != 2) {
      std::cout << "-- Expected to find 2 vertices in the event, but found "
                << ev.particles().size() << std::endl;
      return 1;
    }

    auto parts_s4_p14 = get_parts_with(ev, 4, 14);
    if (parts_s4_p14.size() != 1) {
      std::cout << "-- Expected to find 1 particle with status 4 and pid 14, "
                   "but found "
                << parts_s4_p14.size() << std::endl;
      return 1;
    }
    if (parts_s4_p14.front()->momentum().x() != 1.23) {
      std::cout
          << "-- Expected to find x momentum of particle with status 4 and "
             "pid 14 = 1.23, but found "
          << parts_s4_p14.front()->momentum().x() << std::endl;
      return 1;
    }

    auto parts_s21_p2212 = get_parts_with(ev, 21, 2212);
    if (parts_s21_p2212.size() != 1) {
      std::cout
          << "-- Expected to find 1 particle with status 21 and pid 2212, "
             "but found "
          << parts_s21_p2212.size() << std::endl;
      return 1;
    }
    if (parts_s21_p2212.front()->momentum().z() != 1.23) {
      std::cout
          << "-- Expected to find z momentum of particle with status 21 and "
             "pid 2212 = 1.23, but found "
          << parts_s21_p2212.front()->momentum().z() << std::endl;
      return 1;
    }

    auto parts_s1_p211 = get_parts_with(ev, 1, 211);
    if (parts_s1_p211.size() != 1) {
      std::cout << "-- Expected to find 1 particle with status 1 and pid 211, "
                   "but found "
                << parts_s1_p211.size() << std::endl;
      return 1;
    }
    if (parts_s1_p211.front()->generated_mass() != 1.23) {
      std::cout
          << "-- Expected to find the gen mass of particle with status 1 and "
             "pid 211 = 1.23, but found "
          << parts_s21_p2212.front()->generated_mass() << std::endl;
      return 1;
    }
  }

  if (ev1_list.str() != ev2_list.str()) {
    std::cout << "event listings didn't match but were excepted to:\n"
              << ev1_list.str() << "\n\n"
              << ev2_list.str() << std::endl;
    return 1;
  }

  if (rdr->read_event(ev)) {
    std::cout << "Expected an attempted read of the 3rd event in "
                 "inputIO30.hepmc to fail due to missed whitespace."
              << std::endl;
    return 1;
  }
  return 0;
}
