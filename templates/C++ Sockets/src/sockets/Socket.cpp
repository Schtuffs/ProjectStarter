#include <sockets/Socket.h>

#include <cstring>
#include <print>

// ----- Creation ----- Destruction -----

Socket::Socket(CONNECTION_TYPE cType, uint16_t port) : mType(cType), mPort(port), mSocket(INVALID_SOCKET) {
    mIsSetup = true;
    #ifdef _WIN32
    if (WSAStartup(MAKEWORD(2,2), &mWsaData) != 0) {
        std::print("Failed to start WSA\n");
        mIsSetup = false;
        return;
    }
    #endif
    
    // Create the socket
    if (mType == CONNECTION_TYPE::TCP) {
        mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if (mType == CONNECTION_TYPE::UDP) {
        mSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    
    if (mSocket == INVALID_SOCKET) {
        std::print("ERROR: Failed to create socket\n");
        mIsSetup = false;
        return;
    }
}

Socket::~Socket() {
    #ifdef _WIN32
        WSACleanup();
    #endif

    // Exile the sockets
    CLOSE(mSocket);
}

bool Socket::isSetup() {
    return mIsSetup;
}



// ----- Read -----

bool Socket::poll(SOCKET socket, int millis) {
    pollfd fds[1];
    fds[0].fd = socket;
    fds[0].events = POLLIN;
    return (::POLL(fds, 1, millis) > 0);
}

Packet Socket::receive(int millis, int maxSize) {
    return _receive(mSocket, millis, maxSize);
}

Packet Socket::_receive(SOCKET socket, int millis, int maxSize) {
    // Check if receive possible
    if (!this->poll(socket, millis)) {
        return Packet();
    }
    
    // Receive the head first
    Packet::Head head;
    if (mType == CONNECTION_TYPE::UDP) {
#ifdef _WIN32
        int length = sizeof(mUdpClient);
#else
        socklen_t length = sizeof(mUdpClient);
#endif
        Array<char> buf(maxSize);
        int code = ::recvfrom(socket, buf.data(), maxSize, 0, (sockaddr*)&mUdpClient, &length);
        if (code <= 0) {
            if (code < 0) {
                std::print("ERROR: Failed to receive from socket\n");
            }
            return Packet();
        }
        Packet packet;
        packet.package(buf);
        return packet;
    }
    
    Array<char> headBuf(sizeof(head));
    int code = ::recv(socket, headBuf.data(), sizeof(head), 0);
    if (code <= 0) {
        if (code < 0) {
            std::print("ERROR: Failed to receive head from socket\n");
        }
        return Packet();
    }
    // Copy header data
    std::memcpy(&head, headBuf.data(), headBuf.size());
    
    // Create full buffer
    Array<char> buf(sizeof(head) + head.size);
    std::memcpy(buf.data(), headBuf.data(), headBuf.size());
    
    // Get the rest of the buffer if needed
    if (head.sections > 0) {
        Array<char> sectionBuf(head.size);
        
        // Receive the data
        if (::recv(socket, sectionBuf.data(), sectionBuf.size(), 0) <= 0) {
            std::print("ERROR: Failed to receive section data from socket\n");
            return Packet();
        }
        
        // Add data to buf
        std::memcpy(buf.data() + sizeof(head), sectionBuf.data(), sectionBuf.size());
    }
    
    // Serialize packet to return
    Packet packet;
    packet.package(buf);
    return packet;
}

bool Socket::send(const Packet& data) const noexcept {
    return this->send(data, DEFAULT_IP, mSocket);
}

bool Socket::send(const Packet& data, const std::string& ip) const noexcept {
    return this->send(data, ip, mSocket);
}

bool Socket::send(const Packet& data, const std::string& ip, SOCKET socket) const noexcept {
    if (mType == CONNECTION_TYPE::TCP) {
        return (::send(socket, data.data().data(), data.packetSize(), 0) != SOCKET_ERROR);
    }
    else if (mType == CONNECTION_TYPE::UDP) {
        // define correct variable type for sender address length
        sockaddr_in addr;
        const auto addrLen = sizeof(addr);
        
        // Setup the address
        addr.sin_family = AF_INET;
        addr.sin_port = htons(mPort);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        
        return (::sendto(socket, data.data().data(), data.packetSize(), 0, (sockaddr*)&addr, addrLen) != SOCKET_ERROR);
    }
    return false;
}

