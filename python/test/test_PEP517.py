from pyHepMC3TestUtils import update_path
import sys

sys.path = update_path()


def test_PEP517():
    if sys.version_info < (3, 8):
        print(f"SKIP: importlib.metadata requires Python 3.8+, got {sys.version}")
        return 0    
    import importlib.metadata

    # Test 1: package is discoverable
    try:
        meta = importlib.metadata.metadata("pyHepMC3")
    except importlib.metadata.PackageNotFoundError:
        print("FAIL: pyHepMC3 package metadata not found")
        return 1

    # Test 2: METADATA fields
    if not meta["Metadata-Version"]:
        print("FAIL: Metadata-Version missing")
        return 1
    print(f"OK: Metadata-Version = {meta['Metadata-Version']}")

    if not meta["Name"]:
        print("FAIL: Name missing")
        return 1
    print(f"OK: Name = {meta['Name']}")

    if not meta["Version"]:
        print("FAIL: Version missing")
        return 1
    print(f"OK: Version = {meta['Version']}")

    # Test 3: version is accessible via the standard API
    try:
        version = importlib.metadata.version("pyHepMC3")
        print(f"OK: version = {version}")
    except importlib.metadata.PackageNotFoundError:
        print("FAIL: importlib.metadata.version() failed")
        return 1

    # Test 4: dist-info directory exists and is a dist-info (not egg-info)
    try:
        dist = importlib.metadata.distribution("pyHepMC3")
        dist_path = str(dist._path if hasattr(dist, "_path") else dist.locate_file(""))
        if "egg-info" in dist_path:
            print(f"FAIL: found egg-info instead of dist-info: {dist_path}")
            return 1
        if "dist-info" not in dist_path:
            print(f"FAIL: dist-info directory not found, got: {dist_path}")
            return 1
        print(f"OK: dist-info path = {dist_path}")
    except Exception as e:
        print(f"FAIL: could not locate dist-info: {e}")
        return 1

    # Test 5: WHEEL file exists and contains required fields
    try:
        wheel_text = dist.read_text("WHEEL")
        if wheel_text is None:
            print("FAIL: WHEEL file missing from dist-info")
            return 1
        wheel_lines = dict(
            line.split(": ", 1)
            for line in wheel_text.splitlines()
            if ": " in line
        )
        for required_key in ("Wheel-Version", "Generator", "Root-Is-Purelib", "Tag"):
            if required_key not in wheel_lines:
                print(f"FAIL: WHEEL file missing field: {required_key}")
                return 1
            print(f"OK: WHEEL {required_key} = {wheel_lines[required_key]}")
    except Exception as e:
        print(f"FAIL: could not read WHEEL file: {e}")
        return 1

    # Test 6: INSTALLER file exists and is 'cmake'
    try:
        installer = dist.read_text("INSTALLER")
        if installer is None:
            print("FAIL: INSTALLER file missing from dist-info")
            return 1
        if installer.strip() != "cmake":
            print(f"FAIL: INSTALLER expected 'cmake', got '{installer.strip()}'")
            return 1
        print(f"OK: INSTALLER = {installer.strip()}")
    except Exception as e:
        print(f"FAIL: could not read INSTALLER file: {e}")
        return 1

    # Test 7: RECORD file exists (may be empty stub)
    try:
        record = dist.read_text("RECORD")
        if record is None:
            print("FAIL: RECORD file missing from dist-info")
            return 1
        print(f"OK: RECORD file present (lines: {len(record.splitlines())})")
    except Exception as e:
        print(f"FAIL: could not read RECORD file: {e}")
        return 1

    # Test 8: package is actually importable
    try:
        import pyHepMC3
        print(f"OK: pyHepMC3 importable")
    except ImportError as e:
        print(f"FAIL: pyHepMC3 not importable: {e}")
        return 1

    # Test 9: metadata version matches package version
    try:
        from pyHepMC3 import HepMC3 as hm
        # HepMC3 version should match metadata version
        hepmc3_version = f"{hm.version()}"
        meta_version = importlib.metadata.version("pyHepMC3")
        if hepmc3_version != meta_version:
            print(f"WARN: HepMC3 library version {hepmc3_version} != metadata version {meta_version}")
        else:
            print(f"OK: versions match: {meta_version}")
    except Exception as e:
        print(f"WARN: could not compare versions: {e}")

    print("OK: all PEP517 installation tests passed")
    return 0


if __name__ == "__main__":
    result = 1
    try:
        result = test_PEP517()
    except Exception as e:
        print(f"FAIL: unexpected exception: {e}")
        result = 1
    sys.exit(result)