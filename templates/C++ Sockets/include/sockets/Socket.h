#pragma once

#include <iostream>
#ifdef _WIN32
    #include <winsock2.h>
    #define POLL WSAPoll
    #define CLOSE closesocket
#else
    typedef int SOCKET;
    constexpr int INVALID_SOCKET =  -1;
    constexpr int SOCKET_ERROR =    -1;
    #define CLOSE close
    #include <unistd.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define POLL poll
#endif


#include "Packet.h"

enum class CONNECTION_TYPE  { TCP, UDP };

class Socket {
public:
    static constexpr uint32_t MAX_BUF_SIZE  = 0b11111111'11111111;

    // ----- Read -----
    
    bool poll(SOCKET socket, int millis = 0);
    void send(const Packet& data, const std::string& ip) const noexcept;
    void send(const Packet& data, const std::string& ip, SOCKET socket) const noexcept;
    // Max size only needed for UDP connections
    Packet receive(SOCKET socket, int millis, int maxSize = MAX_BUF_SIZE);
    
    // ----- Update -----
    
protected:
#ifdef _WIN32
    WSADATA mWsaData;
#endif
    SOCKET mSocket;
    CONNECTION_TYPE mType;
    uint16_t mPort;
    sockaddr_in mUdpClient;
    
    // ----- Creation ----- Destruction -----

    Socket(CONNECTION_TYPE cType, uint16_t port);
    ~Socket();
};

