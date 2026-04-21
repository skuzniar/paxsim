#ifndef OUCH_OUCH50_PacketHeader_h
#define OUCH_OUCH50_PacketHeader_h

#include "Types.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct PacketHeader
{
    UInt16 length;
    char   type = 0;

    PacketHeader(char type, uint16_t size)
      : length(size - sizeof(UInt16))
      , type(type)
    {
    }

    std::size_t size() const
    {
        return this->length + sizeof(UInt16);
    }

    const char* data() const
    {
        return reinterpret_cast<const char*>(this);
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const PacketHeader& o)
{
    // clang-format off
    s << "[PacketHeader]=";
    s << "Length: " << o.length << ' ';
    s << "Type: "   << o.type;
    // clang-format on
    return s;
}

#pragma pack(1)
struct SequencedData
{
    static constexpr char Type = 'S';

    PacketHeader header;
    char         type;

    SequencedData(char type, uint16_t size)
      : header(Type, size)
      , type(type)
    {
    }

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const SequencedData& o)
{
    // clang-format off
    s << "[SequencedData]=";
    s << "Header: " << o.header << ' ';
    s << "Type: "   << o.type;
    // clang-format on
    return s;
}

#pragma pack(1)
struct UnsequencedData
{
    static constexpr char Type = 'U';

    PacketHeader header;
    char         type;

    UnsequencedData(char type, uint16_t size)
      : header(Type, size)
      , type(type)
    {
    }

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const UnsequencedData& o)
{
    // clang-format off
    s << "[UnsequencedData]=";
    s << "Header: " << o.header << ' ';
    s << "Type: "   << o.type;
    // clang-format on
    return s;
}

#pragma pack(1)
template<int8_t T, typename V>
struct Appendage
{
    static constexpr int8_t Tag = T;

    uint8_t length = sizeof(tag) + sizeof(value);
    int8_t  tag    = Tag;
    V       value;

    Appendage(const V& v)
      : value(v)
    {
    }

    Appendage(const Appendage& a) = default;
    Appendage(Appendage&& a)      = default;

    static std::size_t size()
    {
        return sizeof(Appendage);
    }
};
#pragma pack()

#pragma pack(1)
struct AppendageHeader
{
    uint8_t length;
    int8_t  tag;

    std::size_t size() const
    {
        return length + sizeof(length);
    }
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
