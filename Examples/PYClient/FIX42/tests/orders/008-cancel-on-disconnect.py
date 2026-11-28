'''O-008'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(client):
    '''Cancel on disconnect'''

    #-- New order
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, client)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(77101))

    client.send(nor)
    client.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    #-- Logout will trigger reconnect
    out = util.fixmsg(util.logout_header, util.logout_body, __doc__, client)

    client.send(out)
    client.expect({35:'5'}, {})

    # Cancel on disconnect
    client.expect({35:'8'}, {39:'4', 150:'4', 11:nor.getField(11), 58:util.has('on disconnect')}, seconds=30)

