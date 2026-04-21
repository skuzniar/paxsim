'''O-001'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Order accepted'''

    #-- New Buy order
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(1601))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41)})

