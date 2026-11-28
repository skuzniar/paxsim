'''O-012'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(client, simctl):
    '''Fills cancelled'''

    # Clear simulator controls
    simctl.send(simctl.Admin.Command({ "Clear":  True }))

    #-- New Buy order with two fills
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, client)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(6104))

    client.send(nor)
    client.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # Two partial fills
    client.expect_all([
        ({35:'8'}, {39:'1', 150:'1', 11:nor.getField(11), 31:'1.230', 32:'101', 151:'6003'}),
        ({35:'8'}, {39:'1', 150:'1', 11:nor.getField(11), 31:'2.340', 32:'202', 151:'5801'})
        ])

    # Two trade cancells
    client.expect_all([
            ({35:'8'}, {39:'1', 150:'5', 11:nor.getField(11), 31:'0.000', 32:'0'}),
            ({35:'8'}, {39:'1', 150:'5', 11:nor.getField(11), 31:'0.000', 32:'0'}),
            ])

    #-- Cancel order
    can = util.fixmsg(util.cancel_order_header, util.cancel_order_body, __doc__, client)
    can.setField(fix.OrigClOrdID(nor.getField(11)))
    can.setField(fix.OrdType(nor.getField(40)))
    can.setField(fix.Side(nor.getField(54)))

    client.send(can)
    client.expect({35:'8'}, {39:'4', 150:'4', 11:can.getField(11), 41:can.getField(41)})

