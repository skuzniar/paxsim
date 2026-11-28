'''O-002'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(client, simctl):
    '''Order rejected'''

    # Clear simulator controls
    simctl.send(simctl.Admin.Command({ "Clear":  True }))

    #  Put blank reject inctructions
    simctl.send(simctl.Order.Reject.Enter({ "Reason":  11, "Text": "SimCtl testing" }))

    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, client)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.123))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(5101))

    client.send(nor)
    client.expect({35:'8'}, {39:'8', 150:'8', 11:nor.getField(11)})

    nor.setField(fix.OrderQty(5888))

    client.send(nor)
    client.expect({35:'8'}, {39:'8', 150:'8', 11:nor.getField(11)})
