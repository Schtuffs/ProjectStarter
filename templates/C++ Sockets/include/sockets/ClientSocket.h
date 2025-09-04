#pragma once

#include "Socket.h"

class ClientSocket : public Socket {
public:
    // ----- Creation ----- Destruction -----

    ClientSocket(CONNECTION_TYPE cType, const std::string& ip, uint16_t port);
    ~ClientSocket();

    // ----- Read -----

    const std::string& ip();
    bool isConnected();
    
    // ----- Update -----

    void ip(const std::string&);

private:
    std::string mIp;
    sockaddr_in mSvrAddr;
    bool mIsConnected;
};

