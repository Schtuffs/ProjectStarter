#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "Socket.h"

class ServerSocket : public Socket {
public:
    // ----- Creation ----- Destruction -----

    ServerSocket(CONNECTION_TYPE cType, uint16_t port);
    ~ServerSocket();

    // ----- Read -----

    // Time in millis to poll for clients
    // Creates new thread for each connected client
    bool accept(int millis = 0);

    // Receive packets from UDP connections
    bool checkSocket(int millis = 0);

    // ----- Update -----

    // Add the receiving lambda for clients
    void addReceive(std::function<void(const Packet&)> function);
    
private:
    // Pointer to a function that the client will call on packet receive
    std::function<void(const Packet&)> mPacketLambda;
    
    sockaddr_in mSvrAddr;
    std::vector<std::thread> mClients;
    static bool isRunning;

    // ----- Read -----
    
    // Each thread is sent here, waits to receive packets
    void receive(SOCKET client);
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    friend BOOL WINAPI EventHandler(DWORD dwType);
#endif
};

