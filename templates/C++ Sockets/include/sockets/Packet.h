#pragma once

#include <functional>
#include <iostream>
#include <string>

void print(const char* data, int n);

class Packet {
public:
    static constexpr int MASK_BODY =   0b10000000'00000000;
    static constexpr int MASK_TAIL =   0b01000000'00000000;
    static constexpr uint64_t ERROR_LENGTH  = 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
#pragma pack (push, 1)
    typedef struct {
        // Packet version, max of 16383 since body and tail bools are put into the first 2 bits
        // If version 0, packet error
        uint16_t version = 0;
        // Length of the body and tail
        uint64_t length = 0;
    } Head;

    typedef struct {
        uint64_t length = 0;
        char* data = nullptr;
    } Body;
    
    typedef struct {
        uint64_t length = 0;
        char* data = nullptr;
    } Tail;
#pragma pack (pop)
    
    Head mHead;
    Body mBody;
    Tail mTail;

    // ----- Creation ----- Destruction -----

    Packet();
    Packet(const Packet& other);
    ~Packet();
    
    // ----- Read -----

    // Returns the internal buffer of the packaged packet
    // Throws std::string if data not packaged beforehand
    const char* data() const;

    // Returns the size of the packet
    int length() const;
    
    // ----- Update -----
    
    // Packs given data buffer into the packet
    void package(const char* buf);
    
    // Creates the packet with its given data
    void package(const Head& head, const Body& body, const Tail& tail);
    void package(const Head& head);
    void package(const Head& head, const Body& body);
    void package(const Head& head, const Tail& tail);

private:
    char* mBuffer;
    int mBufferPointer, mLength;

    void addHead(const Head& head, bool hasBody, bool hasTail);
    void addBody(const Body& body);
    void addTail(const Tail& tail);
};

