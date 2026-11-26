#ifndef OUCH_Modules_OrderFill_dot_h
#define OUCH_Modules_OrderFill_dot_h

#include "Common/Modules/OrderFill.h"

#include "OUCH/Context/OrderBook.h"
#include "OUCH/Context/FillsBook.h"

namespace OUCH::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Order fill handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using OrderFill = Common::Modules::OrderFill<Context::OrderBook, Context::FillsBook, Factory>;

} // namespace OUCH::Modules
#endif
