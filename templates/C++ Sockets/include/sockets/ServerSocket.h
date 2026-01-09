#pragma once

#include <functional>
#include <thread>

#include <sockets/Socket.h>

class ServerSocket : public Socket {
public:
    // ----- Creation ----- Destruction -----

    ServerSocket(CONNECTION_TYPE cType, uint16_t port = DEFAULT_PORT);
    ~ServerSocket();

    // ----- Read -----
    
    // UDP - Receive packets
    // TCP - Accepts new clients
    bool check(int millis = 0, int maxSize = MAX_BUF_SIZE);

    // Checks if servers are still running
    bool isRunning();
    
    // ----- Update -----
    
    // Add the receiving lambda for clients
    void addReceive(std::function<void(const Packet&)> function);

    // Sets the server to be multithreaded and run off the main thread
    // Only works for TCP
    void detach();

    // Receives a packet from the socket
    // Returns 1 if packet received
    // Returns 0 if failure to poll
    // Returns -1 if errors from packet
    int receive(int millis, int maxSize = MAX_BUF_SIZE);

    // Stop the server
    void stop();
    
private:
    // Pointer to a function that the client will call on packet receive
    std::function<void(const Packet&)> mPacketLambda;
    sockaddr_in mSvrAddr;
    std::thread mAcceptThread;
    SOCKET mTempClient;
    bool mIsThreaded;
    
    // ----- Read -----
    
    // Time in millis to poll for clients
    // Creates new thread for each connected client
    bool _accept(int millis = 0, int maxSize = MAX_BUF_SIZE);
    
    // Each thread is sent here, waits to receive packets
    void _clientThread(SOCKET client);

    // Receives a packet from the socket
    // Returns 1 if packet received
    // Returns 0 if failure to poll
    // Returns -1 if errors from packet
    int _receive(SOCKET socket, int millis = 0, int maxSize = MAX_BUF_SIZE);

    // Main loop for threaded server
    void _serverThread();
};

