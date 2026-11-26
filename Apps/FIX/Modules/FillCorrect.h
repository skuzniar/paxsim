#ifndef FIX_Modules_FillCorrect_dot_h
#define FIX_Modules_FillCorrect_dot_h

#include "Common/Modules/FillCorrect.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

namespace FIX::Modules {

//---------------------------------------------------------------------------------------------------------------------
// Fill correct handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
using FillCorrect = Common::Modules::FillCorrect<Context::OrderBook, Context::FillsBook, Factory>;

} // namespace FIX::Modules
#endif
