#include <sockets/ServerSocket.h>

#include <print>

#include <sockets/Packet.h>

static bool s_isRunning = true;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
    BOOL WINAPI EventHandler(DWORD dwType) {
        switch (dwType) {
            case CTRL_C_EVENT:
                std::print("Shutting down server\n");
                s_isRunning = false;

                // Remove event handler incase of freeze
                if (!SetConsoleCtrlHandler(EventHandler, false)) {
                    std::print("Failed to remove Ctrl+C event handler\n");
                }
                return TRUE;
            default:
                break;
        }
        return FALSE;
    }
#endif

// ----- Creation ----- Destruction -----

ServerSocket::ServerSocket(CONNECTION_TYPE cType, uint16_t port) : Socket(cType, port), mIsThreaded(false), mTempClient(INVALID_SOCKET) {
    // Check socket creation
    if (!mIsSetup) {
        return;
    }
    
    // Mount event handler
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(EventHandler, true)) {
        std::print("Failed to set Ctrl+C event handler\n");
    }
#endif

    // Setup server information
    mSvrAddr.sin_family = AF_INET;
    mSvrAddr.sin_addr.s_addr = INADDR_ANY;
    mSvrAddr.sin_port = htons(mPort);

    // Bind the socket
    if (bind(mSocket, (sockaddr*)&mSvrAddr, sizeof(mSvrAddr)) != 0) {
        std::print("ERROR: Failed to bind socket\n");
        mIsSetup = false;
        return;
    }
    
    // TCP server
    if (mType == CONNECTION_TYPE::TCP) {
        // Listen property for socket
        if (listen(mSocket, 10) != 0) {
            std::print("ERROR: Listen failed, could not configure socket\n");
            mIsSetup = false;
            return;
        }
    }
    std::print("{} server bound to port {}\n", (mType == CONNECTION_TYPE::TCP ? "TCP" : "UDP"), mPort);
}

ServerSocket::~ServerSocket() {
    s_isRunning = false;

    // Unmount event handler
#ifdef _WIN32
    SetConsoleCtrlHandler(EventHandler, false);
#endif

    // If threaded
    if (mIsThreaded) {
        mAcceptThread.join();
    } else {
        if (mTempClient != INVALID_SOCKET) {
            CLOSE(mTempClient);
            mTempClient = INVALID_SOCKET;
        }
    }
}



// ----- Read -----

bool ServerSocket::_accept(int millis, int maxSize) {
    // Ensure only used for TCP
    if (mType != CONNECTION_TYPE::TCP) {
        std::print("ERROR: Cannot accept on non-TCP socket\n");
        return false;
    }
    
    // Check socket
    if (!poll(mSocket, millis)) {
        // Nothing found
        return false;
    }
    
    // Delete old client if single threaded
    if (!mIsThreaded) {
        if (mTempClient != INVALID_SOCKET) {
            CLOSE(mTempClient);
            mTempClient = INVALID_SOCKET;
        }
    }

#ifdef _WIN32
    int length = sizeof(mSvrAddr);
#else
    socklen_t length = sizeof(mSvrAddr);
#endif
    if ((mTempClient = ::accept(mSocket, (sockaddr*)&mSvrAddr, &length)) != SOCKET_ERROR) {
        if (mTempClient == INVALID_SOCKET) {
            std::print("ERROR: Could not accept client\n");
            return false;
        }

        // Multithreaded
        if (mIsThreaded) {
            // Detach client thread
            std::thread t(&ServerSocket::_clientThread, this, mTempClient);
            t.detach();
        }
        else {
            // Read until termination
            while (ServerSocket::_receive(mTempClient, millis, maxSize) != -1) {}
        }
        return true;
    }
    return false;
}

bool ServerSocket::check(int millis, int maxSize) {
    // Return if TCP
    if (mType == CONNECTION_TYPE::TCP) {
        // Check for new connections
        return ServerSocket::_accept(millis, maxSize);
    }
    else {
        return (ServerSocket::_receive(mSocket, millis, maxSize) == 1);
    }
}

void ServerSocket::_clientThread(SOCKET client) {
    std::print("Client added on thread {}\n", std::this_thread::get_id());
    
    // All threads are bound to main object lifetime or Ctrl+C
    while (s_isRunning) {
        // Get messages until bad packet received
        if (_receive(client, 100) == -1) {
            break;
        }
    }
    
    std::print("Client ending on thread {}\n", std::this_thread::get_id());
    CLOSE(client);
}

bool ServerSocket::isRunning() {
    return s_isRunning;
}

int ServerSocket::receive(int millis, int maxSize) {
    if (mIsThreaded || mTempClient == INVALID_SOCKET) {
        std::print("ERROR: Server is threaded or no client\n");
        return -1;
    }
    return _receive(mTempClient, millis, maxSize);
}

int ServerSocket::_receive(SOCKET socket, int millis, int maxSize) {
    // Poll
    if (!Socket::poll(socket, millis)) {
        return 0;
    }
    
    // Get packet
    Packet p = Socket::_receive(socket, millis, maxSize);
    
    // Check packet
    if (!p.isValid()) {
        return -1;
    }
    
    // Send packet to user space to determine what to do with it
    mPacketLambda(p);
    return 1;
}

void ServerSocket::_serverThread() {
    std::print("Server running on thread {}\n", std::this_thread::get_id());
    
    // Main loop
    while (s_isRunning) {
        _accept(100);
    }

    std::print("Server closing on thread {}\n", std::this_thread::get_id());
}



// ----- Update -----

void ServerSocket::addReceive(std::function<void(const Packet&)> function) {
    mPacketLambda = function;
}

void ServerSocket::detach() {
    // Ensure TCP
    if (mType != CONNECTION_TYPE::TCP) {
        std::print("ERROR: Threading not allowed for non-TCP socket\n");
        return;
    }

    this->mIsThreaded = true;
    mAcceptThread = std::thread(&ServerSocket::_serverThread, this);
}

void ServerSocket::stop() {
    std::print("Shutting down server\n");
    s_isRunning = false;
}

