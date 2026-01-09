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
constexpr std::string SOCKETS_VERSION   = "2.0.0";
constexpr std::string DEFAULT_IP        = "127.0.0.1";
constexpr uint16_t DEFAULT_PORT         = 34567;

class Socket {
public:
    static constexpr uint32_t MAX_BUF_SIZE  = 0b11111111'11111111;

    // ----- Read -----

    // Call to check that object is properly setup before use
    bool isSetup();
    
    bool poll(SOCKET socket, int millis = 0);

    // Receives packet from own socket
    Packet receive(int millis = 0, int maxSize = MAX_BUF_SIZE);
    
    bool send(const Packet& data) const noexcept;
    bool send(const Packet& data, const std::string& ip) const noexcept;
    bool send(const Packet& data, const std::string& ip, SOCKET socket) const noexcept;
    
protected:
#ifdef _WIN32
WSADATA mWsaData;
#endif
SOCKET mSocket;
CONNECTION_TYPE mType;
    uint16_t mPort;
    sockaddr_in mUdpClient;
    bool mIsSetup;
    
    // ----- Creation ----- Destruction -----
    
    Socket(CONNECTION_TYPE cType, uint16_t port);
    ~Socket();

    // ----- Read -----

    // Max size only needed for UDP connections
    Packet _receive(SOCKET socket, int millis = 0, int maxSize = MAX_BUF_SIZE);
};

