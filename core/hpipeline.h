#ifndef Paxsim_Core_HPipeline_dot_h
#define Paxsim_Core_HPipeline_dot_h

#include "core/streamlog.h"
#include "core/types.h"

#include <cassert>
#include <tuple>
#include <optional>
#include <chrono>
#include <set>

namespace paxsim::core {

//---------------------------------------------------------------------------------------------------------------------
// HPipeline is a collection of message processing modules. Each module receives the incoming message and produces zero
// or more outgoing messages. Timeout event produces zero or more outgoing messages.
//---------------------------------------------------------------------------------------------------------------------
template<typename M, typename... Ms>
class HPipeline
  : public tag::Pipeline
  , public tag::Cyclical
  , public tag::Control
{
public:
    HPipeline()
      : m_modules(make_tuple())
    {
        fill_expset();
    }

    template<typename Context>
    explicit HPipeline(const Context& context)
      : m_modules(make_tuple(context))
    {
        fill_expset();
    }

    // Return the nearest expiration point at which we want the timer to go off
    [[nodiscard]] timepoint timeout_at() const
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        return m_expset.empty() ? timepoint::max() : m_expset.begin()->first;
    }

    // Invoke incoming message notification callback on all of the modules
    template<typename Msg>
    std::vector<std::optional<Msg>> process(const Msg& message)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        std::vector<std::optional<Msg>> vec;
        std::apply([&](auto& p, auto&... ps) { process(vec, message, p, ps...); }, m_modules);
        return vec;
    }

    // Invoke control command notification callback on all of the modules
    template<typename Msg, typename Cmd>
    std::vector<std::optional<Msg>> execute(const Cmd& command)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        std::vector<std::optional<Msg>> vec;
        std::apply([&](auto& p, auto&... ps) { execute(vec, command, p, ps...); }, m_modules);
        return vec;
    }

    // Invoke timer notification callback on the top module.
    template<typename Msg>
    std::vector<std::optional<Msg>> timeout()
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        std::vector<std::optional<Msg>> vec;
        std::apply([&](auto& p, auto&... ps) { timeout(vec, 0UL, p, ps...); }, m_modules);
        return vec;
    }

private:
    // Pass the message throught a series of processing modules
    template<typename Msg, typename Mod, typename... Mods>
    bool process(std::vector<std::optional<Msg>>& vec, const Msg& msg, Mod& mod, Mods&... mods)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        for (auto message : mod.process(msg)) {
            vec.push_back({ message });
            if (!message) {
                return false;
            }
        }
        return process(vec, msg, mods...);
    }

    template<typename Msg, typename Mod>
    bool process(std::vector<std::optional<Msg>>& vec, const Msg& msg, Mod& mod)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        for (auto message : mod.process(msg)) {
            vec.push_back({ message });
            if (!message) {
                return false;
            }
        }
        return true;
    }

    // Pass the command throught a series of processing modules
    template<typename Msg, typename Cmd, typename Mod, typename... Mods>
    bool execute(std::vector<std::optional<Msg>>& vec, const Cmd& cmd, Mod& mod, Mods&... mods)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;

        if constexpr (tag::is_control<std::remove_reference_t<decltype(mod)>>()) {
            for (auto message : execute<Msg, Cmd>(cmd, mod)) {
                vec.push_back({ message });
                if (!message) {
                    return false;
                }
            }
        }
        return execute(vec, cmd, mods...);
    }

    template<typename Msg, typename Cmd, typename Mod>
    bool execute(std::vector<std::optional<Msg>>& vec, const Cmd& cmd, Mod& mod)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;

        if constexpr (tag::is_control<std::remove_reference_t<decltype(mod)>>()) {
            for (auto message : execute<Msg, Cmd>(cmd, mod)) {
                vec.push_back({ message });
                if (!message) {
                    log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << false << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    // Invoke timer notification callback on the top module.
    template<typename Msg, typename Mod, typename... Mods>
    bool timeout(std::vector<std::optional<Msg>>& vec, std::size_t idx, Mod& mod, Mods&... mods)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << idx << std::endl;
        if constexpr (tag::is_cyclical<std::remove_reference_t<decltype(mod)>>()) {
            if (idx == m_expset.begin()->second) {
                for (auto message : timeout<Msg>(mod)) {
                    vec.push_back({ message });
                    if (!message) {
                        reset_expset(mod, idx);
                        return false;
                    }
                }
                reset_expset(mod, idx);
                return true;
            }
        }
        return timeout(vec, ++idx, mods...);
    }

    template<typename Msg, typename Mod>
    bool timeout(std::vector<std::optional<Msg>>& vec, std::size_t idx, Mod& mod)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << idx << std::endl;
        if constexpr (tag::is_cyclical<std::remove_reference_t<decltype(mod)>>()) {
            if (idx == m_expset.begin()->second) {
                for (auto message : timeout<Msg>(mod)) {
                    vec.push_back({ message });
                    if (!message) {
                        reset_expset(mod, idx);
                        return false;
                    }
                }
                reset_expset(mod, idx);
            }
        }
        return true;
    }

    // Invoke timer callback. It is a template function in case of pipeline modules
    template<typename Msg, typename Mod>
    auto timeout(Mod& mod)
    {
        if constexpr (tag::is_pipeline<std::remove_reference_t<decltype(mod)>>()) {
            return mod.template timeout<Msg>();
        } else {
            return mod.timeout();
        }
    }

    // Invoke controll callback. It is a template function in case of pipeline modules
    template<typename Msg, typename Cmd, typename Mod>
    auto execute(const Cmd& cmd, Mod& mod)
    {
        if constexpr (tag::is_pipeline<std::remove_reference_t<decltype(mod)>>()) {
            return mod.template execute<Msg, Cmd>(cmd);
        } else {
            return mod.execute(cmd);
        }
    }

    auto make_tuple()
    {
        return std::make_tuple(std::forward<M>(M()), std::forward<Ms>(Ms())...);
    }

    template<typename Context>
    auto make_tuple(const Context& context)
    {
        return std::make_tuple(std::forward<M>(M(context)), std::forward<Ms>(Ms(context))...);
    }

    // Initialize expiration set for all cyclical modules
    void fill_expset()
    {
        auto emplace = [this](auto now, auto idx, auto const& p) {
            if constexpr (tag::is_cyclical<std::remove_reference_t<decltype(p)>>()) {
                m_expset.emplace(now, idx);
            }
        };

        std::apply(
            [emplace](auto const&... ps) {
                auto now = std::chrono::steady_clock::now();
                auto idx = 0UL;
                ((emplace(now, idx++, ps)), ...);
            },
            m_modules);

        log << level::trace << ts << "--------------------------------" << std::endl;
        for (const auto& pair : m_expset) {
            log << level::trace << ts << pair.first.time_since_epoch().count() << ' ' << pair.second << std::endl;
        }
        log << level::trace << ts << "--------------------------------" << std::endl;
    }

    // Initialize expiration set for all cyclical modules
    template<typename Mod>
    void reset_expset(const Mod& mod, std::size_t idx)
    {
        m_expset.erase(m_expset.begin());
        m_expset.emplace(mod.timeout_at(), idx);
    }

    std::tuple<M, Ms...> m_modules;

    // Timeout management - set of <expiration-timepoint, pipeline-index> records.
    std::set<std::pair<timepoint, std::size_t>> m_expset;
};

} // namespace paxsim::core

#endif
