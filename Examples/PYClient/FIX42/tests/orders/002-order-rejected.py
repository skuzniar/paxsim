'''O-002'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Order rejected'''

    #-- New Buy order rejected because of bad quantity
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.123))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(5101))

    session.send(nor)
    session.expect({35:'8'}, {39:'8', 150:'8', 11:nor.getField(11)})

