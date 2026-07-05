import os
import time
import math
import argparse
import threading
import quickfix

from datetime   import datetime
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

__SOH__ = chr(1)

class FIXApp(quickfix.Application):
    """FIX App"""

    def __init__(self):
        super().__init__()

        self.sessionID = None
        self.UID = int(time.time())

        # Messages queue
        self.iqlock = threading.Lock()
        self.iqueue = []

        # Most relevant tag path and corresponding error message
        self.errtag = None
        self.errmsg = None

    def start(self):
        self.config       = locate([os.getcwd()], "FIXApp.cfg")
        self.settings     = quickfix.SessionSettings(self.config)
        self.storefactory = quickfix.FileStoreFactory(self.settings)
        self.logfactory   = quickfix.FileLogFactory(self.settings)
        self.initiator    = quickfix.SocketInitiator(self, self.storefactory, self.settings, self.logfactory)

        self.initiator.start()

    def stop(self):
        self.initiator.stop()

    def onCreate(self, sessionID):
        print("Created Session: (%s)" % sessionID.toString())
        return

    def onLogon(self, sessionID):
        self.sessionID = sessionID
        print("Successful Logon to session '%s'." % sessionID.toString())
        return

    def onLogout(self, sessionID):
        print("Session (%s) logout!" % sessionID.toString())
        return

    def toAdmin(self, message, sessionID):
        return

    def fromAdmin(self, message, sessionID):
        self.iqlock.acquire()
        try:
            self.iqueue.append(quickfix.Message(message))
            msg = quickfix.Message(message)
            # if msg.getField(35) != "0" and msg.getField(35) != "1":
            print(">>>", message.toString().replace(__SOH__, "|"))
        finally:
            self.iqlock.release()
        return

    def toApp(self, message, sessionID):
        return

    def fromApp(self, message, sessionID):
        self.iqlock.acquire()
        try:
            self.iqueue.append(quickfix.Message(message))
            print(">>>", message.toString().replace(__SOH__, "|"))
        finally:
            self.iqlock.release()
        return

    def onMessage(self, message, sessionID):
        """Processing application message here"""
        pass

    def waitForLogon(self, seconds=10):
        while self.sessionID is None and seconds > 0:
            time.sleep(1)
            seconds -= 1
        return not self.sessionID is None

    def uid(self):
        self.UID += 1
        return self.UID

    def equal(self, lhs, rhs):
        if callable(getattr(lhs, "equal", None)):
            return lhs.equal(rhs)
        if isinstance(lhs, float) and not isinstance(rhs, float):
            return math.isclose(lhs, float(rhs))
        if isinstance(lhs, int) and not isinstance(rhs, int):
            return lhs == int(rhs)
        return lhs == rhs

    def capture(self, tag, msg):
        self.errtag = tag
        self.errmsg = msg

    def clear(self):
        self.errtag = None
        self.errmsg = None

    def fmterror(self):
        return (
            f"Tag: {self.errtag}, error: {self.errmsg}" if self.errtag else f"Error: {self.errmsg}"
        )

    def compare(self, dct, msg):
        for k, v in zip(dct.keys(), dct.values()):
            if msg.isSetField(k):
                f = msg.getField(k)
                if not self.equal(v, f):
                    return False, k, "Expecting " + str(v) + ", got " + str(f)
            else:
                return False, k, "Field " + str(k) + " not found"
        return True, None, None

    def match(self, hdr, bod, seconds):
        return self.match_any([(hdr, bod)], seconds)

    def match_any(self, tuples, seconds):
        dt = 0.1
        while seconds >= 0:
            self.iqlock.acquire()
            try:
                for i, m in enumerate(self.iqueue):
                    for hdr, bod in tuples:
                        retv, errtag, errmsg = self.compare(hdr, m.getHeader())
                        if retv:
                            retv, errtag, errmsg = self.compare(bod, m)
                            if retv:
                                self.iqueue.pop(i)
                                return retv, m
                        self.capture(errtag, errmsg)
            finally:
                self.iqlock.release()
            time.sleep(dt)
            seconds -= dt
            dt = 2 * dt
        return False, None

    def send(self, msg):
        trstime = quickfix.TransactTime()
        trstime.setString(datetime.now().strftime("%Y%m%d-%H:%M:%S.%f")[:-3])
        msg.setField(trstime)

        print("<<<", msg.toString().replace(__SOH__, "|"))

        quickfix.Session.sendToTarget(msg, self.sessionID)

    def purge(self):
        with self.iqlock:
            self.iqueue = []

    def expect(self, hdr, bod, seconds=5):
        self.clear()
        result, message = self.match(hdr, bod, seconds)
        if not result:
            raise Exception(f"Unable to match header:{hdr} body:{bod}. {self.fmterror()}")
        return message

    def expect_any(self, tuples, seconds=5):
        self.clear()
        result, message = self.match_any(tuples, seconds)
        if not result:
            raise Exception(f"Unable to match tuples:{tuples}. {self.fmterror()}")
        return message

    def expect_all(self, tuples, seconds=5):
        for i in range(0, len(tuples)):
            self.expect_any(tuples, seconds)

    # --- Drain message queue for a number of seconds and return last message
    def drain(self, seconds=0):
        m = None
        while seconds >= 0:
            self.iqlock.acquire()
            try:
                while len(self.iqueue) > 0:
                    m = self.iqueue.pop(0)
            finally:
                self.iqlock.release()
            time.sleep(1)
            seconds -= 1
        return m

    # --- Test cases entry points
        '''
    def run(self, path):
        def prompt():
            print("h     - Print this message.")
            print("s     - Scan for test cases.")
            print("l     - List available test cases.")
            print("#|all - Run n-th test case or all.")
            print("q     - Quit.")

        if not self.waitForLogon():
            print("Failed to logon, exiting...")
            # return

        while True:
            try:
                opt = str(input("==> ")).strip()
            except Exception as e:
                break

            if not opt:
                continue
            if opt == "h" or opt == "?":
                prompt()
                continue
            if opt == "s":
                loader.scan()
                print("Loaded {} test cases".format(loader.size()))
                continue
            if opt == "l":
                loader.list()
                continue
            if opt == "q":
                break
            if opt == "all":
                for idx in range(1, loader.size() + 1):
                    self.iqueue.clear()
                    runner.run(loader.file(idx))
                continue
            else:
                try:
                    idx = int(opt)
                    if idx > 0 and idx <= loader.size():
                        self.iqueue.clear()
                        runner.run(loader.file(idx))
                        continue
                except Exception as e:
                    print("Exception occured:", e, traceback.format_exc())
                    continue

                print("Invalid option:", opt)

        # process.kill()
        '''

def main(suite, location):
    try:
        with chdir(location):
            fixapp = FIXApp()
            fixapp.start()
            time.sleep(5)
            fixapp.stop()
    except Exception as e:
        print("Exception occured while running", suite, ':', e)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Loader')
    parser.add_argument('suite', type=str, help='Test suite name')
    parser.add_argument('location', type=str, help='Test suite location')
    args = parser.parse_args()
    main(args.suite, args.location)

