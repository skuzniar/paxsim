import os
import sys
import argparse
import importlib.util

from pathlib    import Path
from contextlib import chdir

class Record:
    def __init__(self, path, mdoc, fdoc):
        self.path = path
        self.mdoc = mdoc
        self.fdoc = fdoc

class Loader:
    def __init__(self):
        self.files    = []
        self.scan()

    def module(self, path):
        spc = importlib.util.spec_from_file_location("module_name", path)
        mod = importlib.util.module_from_spec(spc)
        spc.loader.exec_module(mod)
        return mod

    def scan(self):
        self.files.clear()
        for f in Path(os.getcwd()).rglob("*.py"):
            d = os.path.dirname(f)
            if d not in sys.path:
                sys.path.insert(1, d)
            mod = self.module(f)
            if hasattr(mod, "run"):
                self.files.append(Record(f, mod.__doc__, mod.run.__doc__))
        self.files.sort(key=lambda r: (r.mdoc, r.fdoc))

    def list(self):
        for i in range(0, len(self.files)):
            r = self.files[i]
            if r.mdoc:
                print("%3d - %s: %s" % (i + 1, r.mdoc, r.fdoc if r.fdoc else r.path))
            else:
                print("%3d - %s" % (i + 1, r.fdoc if r.fdoc else r.path))

    def file(self, idx):
        return self.files[idx - 1].path

    def size(self):
        return len(self.files)

def main(suite, location):
    try:
        with chdir(location):
            loader = Loader()
            loader.list()
    except Exception as e:
        print("Exception occured while running", suite, ':', e)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Loader')
    parser.add_argument('suite', type=str, help='Test suite name')
    parser.add_argument('location', type=str, help='Test suite location')
    args = parser.parse_args()
    main(args.suite, args.location)

