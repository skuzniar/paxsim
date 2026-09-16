#ifndef BOE_Traits_h
#define BOE_Traits_h

namespace boe {

//-----------------------------------------------------------------------------------------------------------------
// Message traits.
//-----------------------------------------------------------------------------------------------------------------
template<typename MSG>
class Authentication
{
public:
    static void user(const MSG& msg)
    {
    }
    static void password(const MSG& msg)
    {
    }
};

} // namespace boe

#endif
