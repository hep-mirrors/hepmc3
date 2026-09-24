// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2026 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Test ASCII event positions, position reset and header errors.
//
// CTest runs each case separately:
// - positions: read event and inherited vertex positions correctly.
// - reset: an event without @ gets zero, not the previous event's position.
// - truncated: reject an incomplete position.
// - invalid: reject a position containing a non-number.
// - missing_count: reject a header without the particle count.
// - roundtrip: preserve positions when writing and reading again.
//
// Positions include nonzero -> zero -> nonzero transitions.
// Input files are named inputIO32*.hepmc.
// Run from the build directory:
//   ctest -R '^testIO32_' -V
//
// Reader error messages are expected for the three malformed inputs. Those
// tests pass only when the reader reports failure and returns an empty event.

#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"

using namespace HepMC3;

int check_events(ReaderAscii& reader, std::initializer_list<FourVector> positions, WriterAscii* writer = nullptr) {
    GenEvent event;
    int number = 0;
    for (const auto& position : positions) {
        ++number;
        if (!reader.read_event(event) || reader.failed() || event.event_number() != number ||
            event.particles().size() != 2 || event.vertices().size() != 1 ||
            event.event_pos() != position || event.vertices().front()->position() != position) {
            std::cerr << "Wrong event or position at event " << number << '\n';
            return 1;
        }
        if (writer) writer->write_event(event);
    }
    reader.read_event(event);
    if (!reader.failed()) {
        std::cerr << "Unexpected extra event\n";
        return 1;
    }
    return 0;
}

int run_test(const std::string& scenario) {
    if (scenario == "truncated" || scenario == "invalid" || scenario == "missing_count") {
        const std::string filename = "inputIO32_" + scenario + ".hepmc";
        ReaderAscii reader(filename);
        if (reader.failed()) {
            std::cerr << "Cannot open " << filename << '\n';
            return 1;
        }
        GenEvent event;
        std::cerr << "Expected: read_event=false, failed=true, particles=0, vertices=0.\n"
                  << "Reader error messages below are expected for this malformed input.\n";
        const bool read_ok = reader.read_event(event);
        std::cerr << std::boolalpha << "Observed: read_event=" << read_ok << ", failed=" << reader.failed()
                  << ", particles=" << event.particles().size() << ", vertices=" << event.vertices().size() << ".\n";
        if (read_ok || !reader.failed() ||
            !event.particles().empty() || !event.vertices().empty()) {
            std::cerr << "Expected a failed read and an empty event for: " << filename << '\n';
            return 1;
        }
        return 0;
    }

    if (scenario == "reset") {
        std::cerr << "Expected: the event without @ resets the previous nonzero position to zero.\n";
        ReaderAscii reader("inputIO32_reset.hepmc");
        return check_events(reader, {FourVector(1, 2, 3, 4), FourVector()});
    }
    if (scenario != "positions" && scenario != "roundtrip") {
        std::cerr << "Unknown test scenario: " << scenario << '\n';
        return 1;
    }

    const std::initializer_list<FourVector> positions = {
        FourVector(1, 2, 3, 4), FourVector(-0.5, 0.25, 12, 7), FourVector(), FourVector(5, 6, 7, 8)
    };
    ReaderAscii reader("inputIO32.hepmc");
    if (scenario == "positions") {
        std::cerr << "Expected: all event and inherited vertex positions match the fixture values.\n";
        return check_events(reader, positions);
    }

    std::cerr << "Expected: writing and rereading preserves all event and inherited vertex positions.\n";
    std::stringstream output;
    WriterAscii writer(output);
    if (check_events(reader, positions, &writer)) return 1;
    writer.close();
    ReaderAscii roundtrip(output);
    return check_events(roundtrip, positions);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "FAIL: expected one test scenario\n";
        return 1;
    }
    const int result = run_test(argv[1]);
    std::cout.flush();
    std::cerr << (result == 0 ? "PASS: " : "FAIL: ") << argv[1] << '\n';
    return result;
}
