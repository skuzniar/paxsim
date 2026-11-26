#ifndef FIX_Modules_OrderCancel_dot_h
#define FIX_Modules_OrderCancel_dot_h

#include "Common/Modules/OrderCancel.h"

#include "FIX/Context/OrderBook.h"

namespace FIX::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Order cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using OrderCancel = Common::Modules::OrderCancel<Context::OrderBook, Factory>;

} // namespace FIX::Modules
#endif
