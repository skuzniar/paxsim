'''O-002'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(client):
    '''Order rejected'''

    #-- New Buy order rejected because of bad quantity
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, client)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.123))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(5101))

    client.send(nor)
    client.expect({35:'8'}, {39:'8', 150:'8', 11:nor.getField(11)})

