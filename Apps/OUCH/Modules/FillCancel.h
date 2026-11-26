#ifndef OUCH_Modules_FillCancel_dot_h
#define OUCH_Modules_FillCancel_dot_h

#include "Common/Modules/FillCancel.h"

#include "OUCH/Context/OrderBook.h"
#include "OUCH/Context/FillsBook.h"

namespace OUCH::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Fill cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using FillCancel = Common::Modules::FillCancel<Context::OrderBook, Context::FillsBook, Factory>;

} // namespace OUCH::Modules
#endif
