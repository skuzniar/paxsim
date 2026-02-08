'''O-012'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Fills cancelled'''

    #-- New Buy order with two fills
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(6104))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # Two partial fills
    session.expect_all([
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'1.23', 32:'101', 151:'6003'}),
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'2.34', 32:'202', 151:'5801'})
        ])

    # Two trade cancells
    session.expect_all([
            ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'1.23', 32:'101'}),
            ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'2.34', 32:'202'}),
            ])

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41)})

