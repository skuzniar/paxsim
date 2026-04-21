import os
import sys
import importlib.util

class Record:
    def __init__(self, path, mdoc, fdoc):
        self.path = path
        self.mdoc = mdoc
        self.fdoc = fdoc

class Loader:
    def __init__(self, path):
        sys.path.insert(1, path)
        self.path  = path
        self.files = []
        self.scan()

    def module(self, path):
        spc = importlib.util.spec_from_file_location("module_name", path)
        mod = importlib.util.module_from_spec(spc)

        spc.loader.exec_module(mod)

        return mod

    def scan(self):
        self.files.clear()
        for root, subdirs, files in os.walk(self.path):
            for filename in files:
                if filename.endswith(".py"):
                    path = os.path.join(root, filename)
                    mod  = self.module(path)
                    if hasattr(mod, 'run'):
                        self.files.append(Record(path, mod.__doc__, mod.run.__doc__))
        self.files.sort(key = lambda r: (r.mdoc, r.fdoc))

    def list(self):
        for i in range(0, len(self.files)):
            r = self.files[i]
            if r.mdoc:
                print('%3d - %s: %s' % (i + 1, r.mdoc, r.fdoc if r.fdoc else r.path))
            else:
                print('%3d - %s' % (i + 1, r.fdoc if r.fdoc else r.path))

    def file(self, idx):
        return self.files[idx - 1].path

    def size(self):
        return len(self.files)

