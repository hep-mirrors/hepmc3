from pyHepMC3TestUtils import update_path, python_label
import sys

sys.path = update_path()

from pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3 import HepMC3 as hm

from pyHepMC3.rootIO import HepMC3 as hmrootIO

print(dir(hmrootIO))


def test_IO5():
    input_filename = "inputIO7.root"
    inputA = hmrootIO.ReaderRootTree(input_filename)
    if inputA.failed():
        sys.exit(1)

    # Step 1: Write output by sequentially writing two events we read from input.
    output_name_A = python_label() + "frominputIO7_A.root"
    outputA = hmrootIO.WriterRootTree(output_name_A)
    if outputA.failed():
        sys.exit(2)

    nevents = 2
    counter = 0

    while not inputA.failed():
        if(counter >= nevents): break
        evt = hm.GenEvent()
        inputA.read_event(evt)
        if(inputA.failed()):
            print("End of file reached. Exit.\n")
            nevents = counter
            break

        outputA.write_event(evt)
        evt.clear()

        counter += 1
    inputA.close()
    outputA.close()

    # Step 2: Write identical output, but by utilizing the append functionality.
    #         Start by writing one event less than we want -- we'll then add
    #         that last event with the append.
    inputB = hmrootIO.ReaderRootTree(input_filename)
    if inputB.failed():
        sys.exit(1)

    output_name_B = python_label() + "frominputIO7_B.root"
    outputB = hmrootIO.WriterRootTree(output_name_B)
    if outputB.failed():
        sys.exit(2)

    counter = 0
    while not inputB.failed():
        if(counter >= nevents - 1): break
        evt = hm.GenEvent()
        inputB.read_event(evt)
        if(inputB.failed()):
            print("End of file reached. Exit.\n")
            break

        outputB.write_event(evt)
        evt.clear()
        counter += 1
    outputB.close()

    # open again, now with append
    outputB = hmrootIO.WriterRootTree(output_name_B, append=True)
    if outputB.failed():
        sys.exit(2)

    # write one more event
    evt = hm.GenEvent()
    inputB.read_event(evt)
    if(inputB.failed()):
        sys.exit(2)

    outputB.write_event(evt)
    outputB.close()

    # Step 3: To leverage the ASCII comparison, we convert both outputs to ASCII.
    output_name_A_ascii = output_name_A.replace('.root','.hepmc')
    output_name_B_ascii = output_name_B.replace('.root','.hepmc')

    for input_name,output_name in zip([output_name_A,output_name_B],[output_name_A_ascii,output_name_B_ascii]):
        input = hmrootIO.ReaderRootTree(input_name)
        if(input.failed()):
            sys.exit(2)

        output = hm.WriterAscii(output_name)
        if(output.failed()):
            sys.exit(2)

        while not input.failed():
            evt = hm.GenEvent()
            input.read_event(evt)
            if(input.failed()):
                break
            output.write_event(evt)
            evt.clear()
        input.close()
        output.close()

    # Step 4: Compare the ASCII files.
    assert 0 == COMPARE_ASCII_FILES(output_name_A_ascii, output_name_B_ascii)
    return 0

if __name__ == "__main__":
    result = 1
    try:
        result = test_IO5()
    except:
        result = 1
    sys.exit(result)
