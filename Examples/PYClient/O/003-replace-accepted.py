'''O-003'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Replace accepted'''

    #-- New Buy order
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(2601))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    #-- Replace order
    rep = util.fixmsg(util.replace_order_header, util.replace_order_body, __doc__, session)
    rep.setField(fix.OrigClOrdID(nor.getField(11)))
    rep.setField(fix.OrdType('2'))
    rep.setField(fix.Price(48.22))
    rep.setField(fix.Side('1'))
    rep.setField(fix.OrderQty(2602))

    session.send(rep)
    session.expect({35:'8'}, {39:'5', 150:'5', 11:rep.getField(11), 41:rep.getField(41)})

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(rep.getField(11)))
    can.setField(fix.OrdType(rep.getField(40)))
    can.setField(fix.Side(rep.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41)})

