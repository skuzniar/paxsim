'''O-011'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Order filled'''

    #-- New Buy order with one fill
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(6101))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # One partial fill
    session.expect({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'1.23', 32:'100', 151:'6001'})

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41), 151:'6001'})

    #-- New Buy order with two fills
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(6102))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # Two partial fills
    session.expect_all([
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'1.23', 32:'100', 151:'6002'}),
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'2.34', 32:'200', 151:'5802'})
        ])

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41), 151:'5802'})

    #-- New Buy order with three fills
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(6103))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # Three partial fills
    session.expect_all([
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'1.23', 32:'100', 151:'6003'}),
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'2.34', 32:'200', 151:'5803'}),
        ({35:'8'}, {39:'1', 150:'0', 11:nor.getField(11), 31:'3.45', 32:'300', 151:'5503'})
            ])

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, session)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    session.send(can)
    session.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41), 151:'5503'})

