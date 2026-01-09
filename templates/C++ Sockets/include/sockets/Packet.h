#pragma once

#include <functional>
#include <iostream>
#include <string>

#include <sockets/Array.h>

class Packet {
public:
    static constexpr uint64_t ERROR_SIZE  = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
// #pragma pack (push, 1)
    typedef struct Head {
        // Version of packet, easy for checking client/serverside
        uint16_t version = 0;
        // Size of all sections
        // ERROR_SIZE - invalid packet
        uint64_t size = 0;
        // Total number of sections
        uint64_t sections = 0;
        // Extra packet flags
        uint64_t flags = 0;

        Head(uint16_t version = 0, uint64_t flags = 0);
    } Head;
// #pragma pack (pop)
    
    // ----- Creation ----- Destruction -----

    Packet();
    Packet(const Packet& other);
    ~Packet();
    
    // ----- Read -----

    // ----- Head -----
    
    // Get the packet version
    uint16_t version() const;
    
    // Get all flags from the packet
    uint64_t flags() const;

    // Check for a specific flag on the packet
    bool hasFlag(uint64_t flag) const;

    // Returns the size of the packet body and tail
    uint64_t size() const;
    
    // ----- Sections -----

    // Returns total number of sections
    uint64_t section() const;

    // Returns the section at a given index
    Array<char>& section(uint64_t index) const;

    // ----- Other -----
    
    // Returns the internal buffer of the packaged packet
    Array<char> data() const;

    // Check for after packet creation
    bool isValid() const;

    // Returns total length of the packet
    int packetSize() const;
    
    // ----- Update -----
    
    // Packs given data buffer into the packet
    void package(const Array<char>& buf);
    // Creates head-only packet
    void package(const Head& head);
    // Create packet with head and section array
    void package(const Head& head, const Array<Array<char>>& sections);

private:
    Array<char> mBuffer;
    uint64_t mBufferPointer;
    Head mHead;
    Array<Array<char>> mSections;
};

