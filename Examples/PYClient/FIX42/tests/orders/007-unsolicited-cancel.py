'''O-007'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(client, simctl):
    '''Unsolicited cancel'''

    # Clear simulator controls
    simctl.send(simctl.Admin.Command({ "Clear":  True }))

    #  Put blank reject inctructions
    simctl.send(simctl.Order.Cancel({ "Reason":  11, "Text": "Unsolicited cancel" }))

    #-- New order accepted
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, client)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(7101))

    client.send(nor)
    client.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    # Unsolicited cancel
    client.expect({35:'8'}, {39:'4', 150:'4', 11:nor.getField(11), 58:util.has('Unsolicited cancel')})

