#include <sockets/Packet.h>

#include <cstring>
#include <print>

// ----- Creation ----- Destruction -----

Packet::Packet() {
    mHead.size = ERROR_SIZE;
}

Packet::Packet(const Packet& other) {
    // Copy head
    ::memcpy(&this->mHead, &other.mHead, sizeof(mHead));

    // Copy sections
    mSections = other.mSections;
    
    // Copy main buffer
    mBuffer = other.mBuffer;
}

Packet::Head::Head(uint16_t v, uint64_t f) : version(v), flags(f) {
    // Nothing todo
}

Packet::~Packet() {
    // Nothing todo
}



// ----- Read -----

// ----- Head -----

uint64_t Packet::flags() const {
    return mHead.flags;
}
    
uint16_t Packet::version() const {
    return mHead.version;
}

bool Packet::hasFlag(uint64_t flag) const {
    return mHead.flags & flag;
}

uint64_t Packet::size() const {
    return mHead.size;
}

// ----- Section -----

uint64_t Packet::section() const {
    return mSections.size();
}

Array<char>& Packet::section(uint64_t index) const {
    return mSections.at(index);
}

// ----- Other -----

Array<char> Packet::data() const {
    return mBuffer;
}

bool Packet::isValid() const {
    return (mHead.size != Packet::ERROR_SIZE);
}

int Packet::packetSize() const {
    return mHead.size + sizeof(mHead);
}



// ----- Update -----

void Packet::package(const Array<char>& buf) {
    // Copy head
    std::memcpy(&mHead, buf.data(), sizeof(Head));
    mBuffer = buf;
    mBufferPointer = sizeof(mHead);

    // Copy each section
    mSections = Array<Array<char>>(mHead.sections);
    for (int i = 0; i < mHead.sections; i++) {
        // First param is section size
        uint64_t size = 0;
        std::memcpy(&size, buf.data() + mBufferPointer, sizeof(buf.size()));
        mBufferPointer += sizeof(size);
        if (size == 0) {
            continue;
        }
        
        // Create section array
        mSections[i] = Array<char>(size);
        std::memcpy(mSections[i].data(), buf.data() + mBufferPointer, mSections[i].size());
        mBufferPointer += mSections[i].size();
    }
}

void Packet::package(const Head& head) {
    // Save the head data
    std::memcpy(&mHead, &head, sizeof(mHead));
    mHead.sections = 0;
    mHead.size = 0;
    
    // Prepare buffer
    mBuffer = Array<char>(sizeof(mHead) + mHead.size);
    
    // Copy head into buffer
    std::memcpy(mBuffer.data(), &mHead, sizeof(mHead));
    mBufferPointer = sizeof(mHead);
}

void Packet::package(const Head& head, const Array<Array<char>>& sections) {
    // Save the head data
    std::memcpy(&mHead, &head, sizeof(mHead));
    mHead.sections = sections.size();
    
    // Add size of sections
    mHead.size = 0;
    for (int i = 0; i < sections.size(); i++) {
        mHead.size += sizeof(sections.size());
        mHead.size += sections[i].size();
    }
    
    // Prepare buffer
    mBuffer = Array<char>(sizeof(mHead) + mHead.size);
    
    // Copy head into buffer
    std::memcpy(mBuffer.data(), &mHead, sizeof(mHead));
    mBufferPointer = sizeof(mHead);
    
    // Copy sections
    for (int i = 0; i < sections.size(); i++) {
        // Copy size param
        auto size = sections[i].size();
        std::memcpy(mBuffer.data() + mBufferPointer, &size, sizeof(sections[i].size()));
        mBufferPointer += sizeof(sections[i].size());
        
        // Copy array
        std::memcpy(mBuffer.data() + mBufferPointer, sections[i].data(), sections[i].size());
        mBufferPointer += sections[i].size();
    }
}

