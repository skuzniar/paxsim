import sys
import time

from datetime import datetime

import quickfix as fix

class has:
    def __init__(self, arg):
        self.arg = arg

    def __repr__(self):
        return '<' + self.arg + '>'

    def equal(self, arg):
        return self.arg in arg

def utc_now():
    return datetime.now().strftime("%Y%m%d-%H:%M:%S.%f")[:-3]

def utc_ymd():
    return datetime.now().strftime("%Y%m%d")

def clordid(prefix, session):
    return "%s-%s-%d" % (prefix, utc_ymd(), session.uid())

def fixmsg(header, body, prefix, session):
    msg = fix.Message()
    hdr = msg.getHeader()

    for k, v in zip(header.keys(), header.values()):
        hdr.setField(k, v)

    for k, v in zip(body.keys(), body.values()):
        msg.setField(k, v)

    now = utc_now()
    msg.setField(42,    now)
    msg.setField(60,    now)
    msg.setField(95033, now)

    oid = clordid(prefix, session)
    msg.setField(11,   oid)
    msg.setField(5025, oid)

    msg.setField(58, prefix)

    return msg

new_order_header = {
        8: "FIX.4.4",
       35: "D",
       49: "SenderCompID",
       50: "Internal.Electronic",
       56: "TargetCompID",
      128: "XHEL",
}

new_order_body = {
       1: "RBCALGON9",
      11: "DP01-CS-C251111-0021419",
      12: "0",
      13: "1",
      15: "EUR",
      21: "1",
      22: "4",
      38: "80",
      40: "2",
      42: "20251111-10:38:16.386",
      44: "5.968",
      48: "FI0009000681",
      54: "2",
      55: "NOKIA",
      59: "0",
      60: "20251111-10:38:16.386",
      77: "O",
      81: "0",
      109: "APG",
      120: "EUR",
      155: "1",
      167: "CS",
      204: "0",
      207: "XHEL",
      376: "DP01-CS-CPBB53AA",
      377: "N",
      385: "E",
      529: "8",
      561: "1",
      581: "1",
      847: "1",
}

replace_order_header = {
        8: "FIX.4.4",
       35: "G",
       49: "SenderCompID",
       50: "Internal.Electronic",
       56: "TargetCompID",
      128: "XHEL",
}

replace_order_body = new_order_body

cancel_order_header = {
        8: "FIX.4.4",
       35: "F",
       49: "SenderCompID",
       50: "Internal.Electronic",
       56: "TargetCompID",
      128: "XHEL",
}

cancel_order_body = {
        1: "RBCALGON9",
       11: "DP01-CS-X103817-0021420",
       15: "EUR",
       22: "4",
       37: "497095",
       38: "80",
       40: "2",
       41: "DP01-CS-C251111-0021419",
       44: "5.968",
       48: "FI0009000681",
       54: "2",
       55: "NOKIA",
       60: "20251111-10:38:17.341",
      109: "APG",
      167: "CS",
      207: "XHEL",
      376: "DP01-CS-CPBB53AA",
      581: "1",
      847: "1",
}

