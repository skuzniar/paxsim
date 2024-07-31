#ifndef Simulator_Fix42_Trading_ExecutionBookHandler_dot_h
#define Simulator_Fix42_Trading_ExecutionBookHandler_dot_h

#include "OrderBookHandler.h"

#include <paxsim/core/streamlog.h>

#include <vector>
#include <optional>

namespace Fix42::Trading {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Execution Book Handler. Nothing to do here yet.
//---------------------------------------------------------------------------------------------------------------------
class ExecutionBookHandler
{
public:
    template<typename Context>
    ExecutionBookHandler(Context& context)
      : m_OContext(context)
      , m_SContext(context)
    {
    }

    std::vector<std::optional<FIX::Message>> process(FIX::Message message)
    {
        return {};
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace Fix42::Trading
#endif
