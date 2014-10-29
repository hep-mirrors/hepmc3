#include "RootStreamerTool.h"
using namespace std;

int RootStreamerTool::process(GenEvent &hepmc) {
    HEPMC3CODE(
        IO_RootStreamer st;
        st.write_event(hepmc);
    )

    return 0;
}
