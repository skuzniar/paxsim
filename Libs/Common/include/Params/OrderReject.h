#ifndef Common_Params_OrderReject_dot_h
#define Common_Params_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "Generic.h"
#include <cstddef>

namespace Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order Reject scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderReject
{
public:
    using Config = Common::Config;

    class Enter : public Generic
    {
    public:
        explicit Enter(const Config& config)
        {
            init(config);
        }

        void init(const Config& config)
        {
            if (auto [present, cfg] = config("Control.Order.Reject.Enter"); present) {
                auto count = load(cfg);
                log << level::debug << here << " Loaded " << count << " Reject Enter entries." << std::endl;
            }
        }

        void clear()
        {
            auto count = Generic::clear();
            log << level::debug << here << " Cleared " << count << " Reject Enter entries." << std::endl;
        }
    };

    class Replace : public Generic
    {
    public:
        explicit Replace(const Config& config)
        {
            init(config);
        }

        void init(const Config& config)
        {
            if (auto [present, cfg] = config("Control.Order.Reject.Replace"); present) {
                auto count = load(cfg);
                log << level::debug << here << " Loaded " << count << " Reject Replace entries." << std::endl;
            }
        }

        void clear()
        {
            auto count = Generic::clear();
            log << level::debug << here << " Cleared " << count << " Reject Replace entries." << std::endl;
        }
    };

    class Cancel : public Generic
    {
    public:
        explicit Cancel(const Config& config)
        {
            init(config);
        }

        void init(const Config& config)
        {
            if (auto [present, cfg] = config("Control.Order.Reject.Cancel"); present) {
                auto count = load(cfg);
                log << level::debug << here << " Loaded " << count << " Reject Cancel entries." << std::endl;
            }
        }

        void clear()
        {
            auto count = Generic::clear();
            log << level::debug << here << " Cleared " << count << " Reject Cancel entries." << std::endl;
        }
    };

    explicit OrderReject(const Config& config)
      : enter(config)
      , replace(config)
      , cancel(config)
    {
    }

    void init(const Config& config)
    {
        enter.init(config);
        replace.init(config);
        cancel.init(config);
    }

    void clear()
    {
        enter.clear();
        replace.clear();
        cancel.clear();
    }

    Enter   enter;
    Replace replace;
    Cancel  cancel;
};

/*
//---------------------------------------------------------------------------------------------------------------------
// Order Reject Replace scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderRejectReplace : public Generic<Common::Config>
{
public:
    explicit OrderRejectReplace(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Order.Reject.Replace"); present) {
            load(cfg);
        }
    }
};

//---------------------------------------------------------------------------------------------------------------------
// Order Reject Cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderRejectCancel : public Generic<Common::Config>
{
public:
    explicit OrderRejectCancel(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Order.Reject.Cancel"); present) {
            load(cfg);
        }
    }
};
*/

} // namespace Params
#endif
