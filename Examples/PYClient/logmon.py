import os
import json
import time
import glob
import argparse
import threading

from contextlib import chdir

class Logmon:
    def __init__(self):
        # List of files and their modification times
        self.files = []
        self.times = []

        # Log messages queue
        self.lqlock = threading.Lock()
        self.lqueue = []

        # Most relevant matching path and coresponding error message
        self.errpath = []
        self.errmesg = None

        #--- Find the log files the current location
        self.logs = glob.glob(os.getcwd() + '/' + '**/*.log', recursive=True)

    def start(self):
        for log in self.logs:
            file = open(log, 'r')
            self.callback(file.readlines())
            self.files.append(file)
            self.times.append(0)
            print("Opened:", log, file.fileno())

        # Thread control
        self.run = True
        self.thread = threading.Thread(target=self.monitor)
        self.thread.start()

    def stop(self):
        for file in self.files:
            print("Closed:", file.fileno())
            file.close()
        self.run = False
        self.thread.join()

    def monitor(self):
        print("Starting monitor run...")
        while self.run:
            for i, (f, t) in enumerate(zip(self.files, self.times)):
                mt = self.modtime(f)
                if t < mt:
                    #print("File changed:", f.fileno())
                    self.callback(f.readlines())
                    self.times[i] = mt
            time.sleep(0.1)
        print("Stopping monitor run...")

    def modtime(self, fileobj):
        info = os.fstat(fileobj.fileno())
        return info.st_mtime
    
    def callback(self, lines):
        with self.lqlock:
            for line in lines:
                #print("------------:", len(self.lqueue), ' ', line)
                try:
                    self.lqueue.append(json.loads(line.strip()))
                except json.JSONDecodeError as e:
                    pass

    def purge(self):
        with self.lqlock:
            self.lqueue = []

    def equal(self, lhs, rhs):
        if (callable(getattr(lhs, 'equal', None))):
            return lhs.equal(rhs)
        return lhs == rhs

    def find_sub_object(self, obj, key):
        if isinstance(obj, dict):
            if key in obj:
                return obj[key]
            for v in obj.values():
                result = self.find_sub_object(v, key)
                if result is not None:
                    return result
        return None

    def clear(self):
        self.errpath = []
        self.errmesg = None

    def capture(self, path, mesg):
        if not self.errpath or len(self.errpath) < len(path):
            self.errpath = path
            self.errmesg = mesg

    def fmterror(self):
        return f"Path: {'.'.join(self.errpath)}, error: {self.errmesg}" if self.errpath else f"Error: {self.errmesg}"

    def compare(self, dct, obj, path):
        if isinstance(dct, dict):
            for k, v in dct.items():
                sub = self.find_sub_object(obj, k)
                if not sub:
                    return False, path, "Element " + k + " not found"
                path.append(k)
                retv, retp, retm = self.compare(v, sub, path)
                if not retv:
                    return retv, retp, retm
                path.pop()
            return True, path, None
        return self.equal(dct, obj), path, "Expecting " + str(dct) + ", got " + str(obj)

    def match(self, dct, seconds):
        return self.match_any([dct], seconds)

    def match_any(self, dcts, seconds):
        dt = 0.1
        while seconds >= 0:
            with self.lqlock:
                for i, msg in enumerate(self.lqueue):
                    for dct in dcts:
                        retv, path, errm = self.compare(dct, msg, [])
                        if retv:
                            self.lqueue.pop(i)
                            return retv, msg
                        self.capture(path, errm)

            time.sleep(dt)
            seconds -= dt
            dt = 2 * dt
        return False, None

    def expect(self, dct, seconds=5):
        self.clear()
        result, message = self.match(dct, seconds)
        if not result:
            raise Exception(f"Unable to match: {dct}. {self.fmterror()}")
        return message

    def expect_any(self, dcts, seconds=5):
        self.clear()
        result, message = self.match_any(dcts, seconds)
        if not result:
            raise Exception(f"Unable to match: {dcts}. Path: {'.'.join(self.errpath)}, error: {self.errmesg}")
        return message

    def expect_all(self, tuples, seconds=5):
        for i in range(0, len(tuples)):
            self.expect_any(tuples, seconds)


def main(suite, location):
    try:
        with chdir(location):
            logmon = Logmon()
            logmon.start()
            time.sleep(10)
            logmon.stop()
    except Exception as e:
        print("Exception occured while running", suite, ':', e)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Log monitor')
    parser.add_argument('suite', type=str, help='Test suite name')
    parser.add_argument('location', type=str, help='Test suite location')
    args = parser.parse_args()
    main(args.suite, args.location)

