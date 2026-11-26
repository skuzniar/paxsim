#ifndef OUCH_Modules_OrderCancel_dot_h
#define OUCH_Modules_OrderCancel_dot_h

#include "Common/Modules/OrderCancel.h"

#include "OUCH/Context/OrderBook.h"

namespace OUCH::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Order cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using OrderCancel = Common::Modules::OrderCancel<Context::OrderBook, Factory>;

} // namespace OUCH::Modules
#endif
