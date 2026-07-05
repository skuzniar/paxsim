import os
import time
import argparse
import traceback

from pathlib    import Path
from contextlib import chdir

from proctl import Proctl
from fixapp import FIXApp
from logmon import Logmon
from loader import Loader
from runner import Runner

class TheApp:
    def __init__(self, root):
        self.loctuples = self.locations(root)

    def locations(self, root):
        paths = [
                '~/git/theapp',
            ]
        paths.insert(0, root)

        #--- Find the set of testable locations
        files = []
        for p in paths:
            for f in Path(os.path.expanduser(p)).rglob('.' + type(self).__name__.lower()):
                files.append(str(f))

        #--- Loctuples is a dictionary of test suite names and their locations
        loctuples = dict()
        for f in files:
            spath = os.path.dirname(f)
            with open(f, 'r') as file:
                for line in file.readlines():
                    name =  line.strip()
                    if len(name) > 0 and not name.startswith('#'):
                        loctuples[name] = spath if spath else '.'

        return loctuples

    # --- Test cases entry points
    def run(self):
        """Run"""

        if len(self.loctuples) == 0:
            print("Nothing to do. Exiting...")
            return

        # Pretend we chose the suite to run
        suite, location = next(iter(self.loctuples.items()))
        print("Selected suite:", suite, "at:", location)

        with chdir(location):
            proctl = Proctl()
            fixapp = FIXApp()
            logmon = Logmon()
            loader = Loader()

            proctl.start()
            fixapp.start()
            logmon.start()

            self.interact(loader, fixapp, logmon)

            logmon.stop()
            fixapp.stop()
            proctl.stop()

    def interact(self, loader, fixapp, logmon):
        def prompt():
            print("h     - Print this message.")
            print("s     - Scan for test cases.")
            print("l     - List available test cases.")
            print("#|all - Run n-th test case or all.")
            print("q     - Quit.")

        if not fixapp.waitForLogon():
            print("Failed to logon, exiting...")
            return

        runner = Runner(fixapp, logmon)

        while True:
            try:
                opt = str(input("==> ")).strip()
            except Exception as e:
                break

            if not opt:
                continue
            if  opt == 'h' or opt == '?':
                prompt()
                continue
            if  opt == 's':
                loader.scan()
                print("Loaded {} test cases".format(loader.size()))
                continue
            if  opt == 'l':
                loader.list()
                continue
            if  opt == 'q':
                break
            if  opt == 'all':
                for idx in range(1, loader.size() + 1):
                    runner.run(loader.file(idx))
                continue
            else:
                try:
                    idx = int(opt)
                    if idx > 0 and idx <= loader.size():
                        runner.run(loader.file(idx))
                        continue
                except Exception as e:
                    print("Exception occured:", e, traceback.format_exc())
                    continue

                print("Invalid option:", opt)

def main(root):
    try:
        theapp = TheApp(root)
        theapp.run()
    except Exception as e:
        print("Exception occured:", e)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='The App')
    parser.add_argument('--root', type=str, default='.', help='Root directory containing test suite')
    args = parser.parse_args()
    main(args.root)
