#ifndef FIX_Modules_OrderFill_dot_h
#define FIX_Modules_OrderFill_dot_h

#include "Common/Modules/OrderFill.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

namespace FIX::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Order fill handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using OrderFill = Common::Modules::OrderFill<Context::OrderBook, Context::FillsBook, Factory>;

} // namespace FIX::Modules
#endif
