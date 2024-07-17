#ifndef Paxsim_Core_RWBuffer_dot_h
#define Paxsim_Core_RWBuffer_dot_h

#include <cstring>
#include <memory>

namespace paxsim::core {

struct RWBuffer
{
    char* wpos()
    {
        return std::addressof(m_data[0]) + m_woff;
    }
    char* rpos()
    {
        return std::addressof(m_data[0]) + m_roff;
    }
    [[nodiscard]] const char* rpos() const
    {
        return std::addressof(m_data[0]) + m_roff;
    }

    [[nodiscard]] std::size_t wsize() const
    {
        return sizeof(m_data) - m_woff;
    }
    [[nodiscard]] std::size_t rsize() const
    {
        return m_woff - m_roff;
    }

    [[nodiscard]] std::size_t woff() const
    {
        return m_woff;
    }
    [[nodiscard]] std::size_t roff() const
    {
        return m_roff;
    }

    std::size_t produced(std::size_t count)
    {
        m_woff += count;
        return count;
    }
    std::size_t consumed(std::size_t count)
    {
        m_roff += count;
        return count;
    }

    [[nodiscard]] bool above_hw_mark() const
    {
        return m_woff > m_hwmk;
    }

    void reset()
    {
        if (m_roff == m_woff) {
            m_roff = m_woff = 0;
        } else {
            std::memmove(std::addressof(m_data[0]), rpos(), rsize());
            m_woff -= m_roff;
            m_roff = 0;
        }
    }

private:
    static constexpr std::size_t N = 100L * 1024; // TODO - parameterize

    char        m_data[N];
    std::size_t m_hwmk = N * 0.75; // TODO - parameterize

    std::size_t m_woff = 0;
    std::size_t m_roff = 0;
};

} // namespace paxsim::core

#endif
