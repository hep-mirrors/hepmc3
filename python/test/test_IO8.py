from pyHepMC3TestUtils import update_path, python_label
#Round-trip conversion: ASCII files --> compressed ASCII --> ASCII files for multiple compression algorithms.
import sys

sys.path = update_path()

from pyHepMC3TestUtils import COMPARE_ASCII_FILES
from pyHepMC3 import HepMC3 as hm


def test_IO81(ext, form):
    input_path = "inputIO8.hepmc"
    output_path = python_label() + "frominputIO8.hepmc" + ext
    print(f"test_IO81: reading {input_path}, writing {output_path} using format={form}")
    inputA = hm.ReaderAsciiHepMC2(input_path)
    if inputA is None or inputA.failed():
        print(f"test_IO81: failed to open input reader for extension {ext} (format={form})")
        sys.exit(1)
    outputA = hm.WriterGZ(hm.WriterAscii, output_path, form)
    if outputA is None or outputA.failed():
        print(f"test_IO81: failed to create output writer for extension {ext} (format={form})")
        sys.exit(12)
    while not inputA.failed():
        evt = hm.GenEvent()
        inputA.read_event(evt)
        if inputA.failed():
            print("End of file reached. Exit.\n")
            break
        outputA.write_event(evt)
        evt.clear()
    inputA.close()
    outputA.close()


def test_IO82(ext, form):
    input_path = python_label() + "frominputIO8.hepmc" + ext
    output_path = python_label() + "fromfrominputIO8" + ext + ".hepmc"
    print(f"test_IO82: reading {input_path}, writing {output_path} using format={form}")
    inputB = hm.ReaderGZ(hm.ReaderAscii, input_path, form)
    if inputB is None or inputB.failed():
        print(f"test_IO82: failed to create input reader for extension {ext} (format={form})")
        sys.exit(3)
    outputB = hm.WriterAsciiHepMC2(output_path)
    if outputB is None or outputB.failed():
        print(f"test_IO82: failed to create output writer for extension {ext} (format={form})")
        sys.exit(4)
    while not inputB.failed():
        evt = hm.GenEvent()
        inputB.read_event(evt)
        print(evt.event_number())
        if inputB.failed():
            print("End of file reached. Exit.\n")
            break
        outputB.write_event(evt)
        evt.clear()
    inputB.close()
    outputB.close()
    assert 0 == COMPARE_ASCII_FILES(python_label() + "fromfrominputIO8" + ext + ".hepmc", "inputIO8.hepmc")
    return 0


def available_formats():
    formats = [('.gz', 'gzip')]

    try:
        import bz2  # noqa: F401
        formats.append(('.bz2', 'bz2'))
    except ImportError:
        pass

    try:
        import lzma  # noqa: F401
        formats.append(('.lzma', 'lzma'))
    except ImportError:
        pass

# 
    try:
        import zstandard  # noqa: F401
        # zstandard is a third-party module; only add support if it exposes open()
        if hasattr(zstandard, 'open'):
            formats.append(('.z', 'zstandard'))
    except ImportError:
        pass

    return formats


def test_IO8():
    result = 0
    for ext, form in available_formats():
        try:
            test_IO81(ext, form)
            code = test_IO82(ext, form)
        except SystemExit as e:
            print(f"test_IO8: failure for extension {ext} (format={form}), exit code={e.code}")
            result += 1
            continue
        except Exception as e:
            print(f"test_IO8: failure for extension {ext} (format={form}), exception={e}")
            result += 1
            continue

        if code != 0:
            print(f"test_IO8: nonzero result for extension {ext} (format={form}): {code}")
            result += code

    if result != 0:
        print(f"test_IO8: completed with {result} failed format(s)")
    return result


if __name__ == '__main__':
    result = 1
    try:
        result = test_IO8()
    except Exception as e:
        print(f"test_IO8: unexpected exception: {e}")
        result = 1
    sys.exit(result)
