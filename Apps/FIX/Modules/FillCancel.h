#ifndef FIX_Modules_FillCancel_dot_h
#define FIX_Modules_FillCancel_dot_h

#include "Common/Modules/FillCancel.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

namespace FIX::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Fill cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using FillCancel = Common::Modules::FillCancel<Context::OrderBook, Context::FillsBook, Factory>;

} // namespace FIX::Modules
#endif
