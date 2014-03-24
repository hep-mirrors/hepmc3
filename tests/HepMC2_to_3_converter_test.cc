/**
 * @brief Test of HepMC2 to 3 IO file converter
 *
 * @author Tomasz Przedzinski
 * @date 21th March 2014
 */
#include <iostream>
#include "HepMC3/IO_HepMC2_adapter.h"
#include "HepMC3/IO_GenEvent.h"
#include "HepMC3/GenEvent.h"
using namespace HepMC3;
using std::cout;
using std::endl;

int main() {
    IO_HepMC2_adapter  adapter("../../HepMC3-benchmark-files/tau.Z.Photos.hepmc");
    IO_GenEvent        output_file("test.hepmc",std::ios::out);
    int events_parsed = 0;

    while(!adapter.rdstate()) {
        if(events_parsed%100==0) cout<<"Events parsed: "<<events_parsed<<endl;

        GenEvent *evt = new GenEvent();

        adapter.fill_next_event(evt);
        output_file.write_event(evt);
        if(events_parsed==0) {
            evt->print();
        }

        delete evt;

        events_parsed++;
    }

    adapter.close();
    output_file.close();

    return 0;
}
