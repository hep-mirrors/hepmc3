from pyHepMC3TestUtils import update_path, python_label
import sys

sys.path = update_path()

from pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3 import HepMC3 as hm


def test_IO10():
    inputA = hm.ReaderMT(hm.ReaderAsciiHepMC2, "inputIO10.hepmc", 3)
    inputA_events = []
    if inputA.failed():
        sys.exit(1)
    while not inputA.failed():
        evt = hm.GenEvent()
        inputA.read_event(evt)
        if inputA.failed():
            print("End of file reached. Exit.\n")
            break
        evt.set_run_info(None)
        inputA_events.append(evt)
    inputA.close()

    outputA = hm.WriterAscii(python_label() + "frominputIO10.hepmc")
    if outputA.failed():
        sys.exit(2)
    for e in sorted(inputA_events, key=lambda e: e.event_number()):
        outputA.write_event(e)
    outputA.close()
    inputA_events.clear()

    inputB = hm.ReaderMT(hm.ReaderAscii, python_label() + "frominputIO10.hepmc", 2)
    inputB_events = []
    if inputB.failed():
        sys.exit(3)
    while not inputB.failed():
        evt = hm.GenEvent()
        inputB.read_event(evt)
        if inputB.failed():
            print("End of file reached. Exit.\n")
            break
        inputB_events.append(evt)
    inputB.close()

    outputB = hm.WriterAsciiHepMC2(python_label() + "fromfrominputIO10.hepmc")
    if outputB.failed():
        sys.exit(4)
    for e in sorted(inputB_events, key=lambda e: e.event_number()):
        outputB.write_event(e)
    outputB.close()
    inputB_events.clear()

    assert 0 == COMPARE_ASCII_FILES(python_label() + "fromfrominputIO10.hepmc", "inputIO10.hepmc")
    return 0


if __name__ == "__main__":
    result = 1
    try:
        result = test_IO10()
    except Exception:
        result = 1
    sys.exit(result)
