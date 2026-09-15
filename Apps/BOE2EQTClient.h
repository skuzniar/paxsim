#ifndef BOE2Client_dot_h
#define BOE2Client_dot_h

#include "BOE/BOE2Client.h"

#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Connector.h"

#include <unistd.h>

using namespace PaxSim;

//---------------------------------------------------------------------------------------------------------------------
// Self contained client class.
//---------------------------------------------------------------------------------------------------------------------
struct Client : public boe::boe2::Client
{
public:
    Client(const std::string& host, int port, const std::string& user, const std::string& password)
      : m_apcontext(user, password)
      , m_connector(m_iocontext, host, port, 3, 3)
    {
    }

    ~Client()
    {
        stop();
    }

    void start()
    {
        m_connector.connect(m_apcontext);
        m_thread = std::thread([&]() { m_iocontext.run(); });
    }

    void stop()
    {
        m_iocontext.stop();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    void post()
    {
        m_iocontext.post([]() { std::cout << "================= post" << std::endl; });
    }

private:
    using Handler = boe::boe2::Client::Handler;

    Core::IOContext          m_iocontext;
    Context::Session         m_apcontext;
    Core::Connector<Handler> m_connector;
    std::thread              m_thread;
};

#endif
