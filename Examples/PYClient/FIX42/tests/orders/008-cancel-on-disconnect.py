'''O-008'''

import sys
import time

import quickfix  as fix
import impl.util as util

#--- Test case must have run method
def run(session):
    '''Cancel on disconnect'''

    #-- New order
    nor = util.fixmsg(util.new_order_header, util.new_order_body, __doc__, session)
    nor.setField(fix.OrdType('2'))
    nor.setField(fix.Price(48.20))
    nor.setField(fix.Side('1'))
    nor.setField(fix.OrderQty(77101))

    session.send(nor)
    session.expect({35:'8'}, {39:'0', 150:'0', 11:nor.getField(11)})

    #-- Logout will trigger reconnect
    out = util.fixmsg(util.logout_header, util.logout_body, __doc__, session)

    session.send(out)
    session.expect({35:'5'}, {})

    # Cancel on disconnect
    session.expect({35:'8'}, {39:'4', 150:'4', 11:nor.getField(11), 58:util.has('on disconnect')}, seconds=30)

