import os
import time
import argparse
import subprocess
import selectors
import threading

from pathlib    import Path
from contextlib import chdir

def locate(paths, pattern):
    files = []
    for p in paths:
        for f in Path(os.path.expanduser(p)).rglob(pattern):
            if Path.is_file(f):
                files.append(f)
    if not files:
        raise Exception(f"Unable to locate: {pattern} in: {paths}.")
    return str(max(files, key=os.path.getctime)) if files else None

#--------------------------------------------------------------------------------------------------
# Process controller. Runs processes it deems necessary. Here just exchange simulator.
#--------------------------------------------------------------------------------------------------
class Proctl:
    def __init__(self):
        pass

    # Simulator command
    def scmd(self):
        paths = [
                '~/git/paxsim',
                '~/dvlp/paxsim',
           ]

        sexe = locate(paths, "PAXSim")
        scfg = locate([os.getcwd()], "PAXSim.cfg")

        cmd = None
        if sexe and scfg:
            cmd = sexe + " -c " + scfg
        return cmd

    def start(self):
        cmds = [ self.scmd(), ]

        self.procs = []
        self.sells = selectors.DefaultSelector()

        for cmd in cmds:
            print("Starting: ", cmd)

            p =subprocess.Popen(
                cmd.split(),
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                universal_newlines=True,
                bufsize=1,
                text=True
            )

            os.set_blocking(p.stdout.fileno(), False)
            self.procs.append(p)
            self.sells.register(p.stdout, selectors.EVENT_READ)

        # Thread control
        self.run = True
        self.thread = threading.Thread(target=self.monitor)
        self.thread.start()

    def stop(self):
        self.run = False
        self.thread.join()

        self.sells.close()
        for proc in self.procs:
            print("Stopping", proc.pid)
            proc.kill()
            '''
            outs, errs = proc.communicate()
            if outs:
                print(proc.pid, outs)
            if errs:
                print(proc.pid, errs)
            '''

    def monitor(self):
        print("Starting monitor run...")
        while self.run:
            events = self.sells.select(1)
            for key, mask in events:
                for l in key.fileobj.readlines():
                    print(l)

            for proc in list(self.procs):
                if proc.poll() != None:
                    outs, errs = proc.communicate()
                    if outs:
                        print(proc.pid, outs)
                    if errs:
                        print(proc.pid, errs)
                    self.procs.remove(proc)
                    self.sells.unregister(proc.stdout)
        print("Stopping monitor run...")

def main(suite, location):
    try:
        with chdir(location):
            proctl = Proctl()
            proctl.start()
            time.sleep(5)
            proctl.stop()
    except Exception as e:
        print("Exception occured while running", suite, ':', e)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Process control')
    parser.add_argument('suite', type=str, help='Test suite name')
    parser.add_argument('location', type=str, help='Test suite location')
    args = parser.parse_args()
    main(args.suite, args.location)

