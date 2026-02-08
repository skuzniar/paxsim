"""FIX Application"""
import sys
import quickfix as fix
import time
import logging
import traceback
import threading

from datetime import datetime

from loader import Loader
from runner import Runner

__SOH__ = chr(1)

class Application(fix.Application):
    """FIX Application"""
    ClOrdID   = 0
    sessionID = None
    UID       = int(time.time())

    iqlock    = threading.Lock()
    iqueue    = []

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
            self.iqueue.append(fix.Message(message))
            msg = fix.Message(message)
            #if msg.getField(35) != "0" and msg.getField(35) != "1":
            print(">>>", message.toString().replace(__SOH__, "|"))
        finally:
            self.iqlock.release()
        return
    def toApp(self, message, sessionID):
        return
    def fromApp(self, message, sessionID):
        self.iqlock.acquire()
        try:
            self.iqueue.append(fix.Message(message))
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
        if (callable(getattr(lhs, 'equal', None))):
            return lhs.equal(rhs)
        return lhs == rhs

    def compare(self, dct, msg):
        for k, v in zip(dct.keys(), dct.values()):
            if msg.isSetField(k):
                f = msg.getField(k)
                if not self.equal(v, f):
                    return False
            else:
                return False
        return True

    def match(self, hdr, bod, seconds):
        while seconds >= 0:
            self.iqlock.acquire()
            try:
                for i, m in enumerate(self.iqueue):
                    if self.compare(hdr, m.getHeader()) and self.compare(bod, m):
                        self.iqueue.pop(i)
                        return True, m
            finally:
                self.iqlock.release()
            time.sleep(1)
            seconds -= 1
        return False, None

    def match_any(self, tuples, seconds):
        while seconds >= 0:
            self.iqlock.acquire()
            try:
                for i, m in enumerate(self.iqueue):
                    for hdr, bod in tuples:
                        if self.compare(hdr, m.getHeader()) and self.compare(bod, m):
                            self.iqueue.pop(i)
                            return True, m
            finally:
                self.iqlock.release()
            time.sleep(1)
            seconds -= 1
        return False, None

    def send(self, msg):
        trstime = fix.TransactTime()
        trstime.setString(datetime.now().strftime("%Y%m%d-%H:%M:%S.%f")[:-3])
        msg.setField(trstime)

        print("<<<", msg.toString().replace(__SOH__, "|"))

        fix.Session.sendToTarget(msg, self.sessionID)

    def expect(self, hdr, bod, seconds=5):
        result, message = self.match(hdr, bod, seconds)
        if not result:
            raise Exception(f"Unable to match header:{hdr} body:{bod}")
        return message

    def expect_any(self, tuples, seconds=5):
        result, message = self.match_any(tuples, seconds)
        if not result:
            raise Exception(f"Unable to match tuples:{tuples}")
        return message

    def expect_all(self, tuples, seconds=5):
        for i in range(0, len(tuples)):
            self.expect_any(tuples, seconds)

    #--- Drain message queue for a number of seconds and return last message
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

    #--- Test cases entry points
    def run(self, path):
        """Run"""
        loader = Loader(path)
        runner = Runner(self)

        def prompt():
            print("h     - Print this message.")
            print("s     - Scan for test cases.")
            print("l     - List available test cases.")
            print("#|all - Run n-th test case or all.")
            print("q     - Quit.")

        if not self.waitForLogon():
            print("Failed to logon, exiting...")
            return

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

