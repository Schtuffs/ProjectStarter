#pragma once

#include <sockets/Socket.h>

class ClientSocket : public Socket {
public:
    // ----- Creation ----- Destruction -----

    ClientSocket(CONNECTION_TYPE cType, const std::string& ip = DEFAULT_IP, uint16_t port = DEFAULT_PORT);
    ~ClientSocket();

    // ----- Read -----

    const std::string& ip();
    
    // ----- Update -----

    void ip(const std::string&);

private:
    std::string mIp;
    sockaddr_in mSvrAddr;
};

