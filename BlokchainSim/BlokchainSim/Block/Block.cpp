#include "Block.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

Block::Block(unsigned int index,
    const std::string& data,
    const std::string& prevHash)
    : m_index(index),
    m_timestamp(std::time(nullptr)),
    m_data(data),
    m_prevHash(prevHash),
    m_nonce(0) // initialize
{
    generateHash();
}

void Block::generateHash()
{
    m_hash = calculateHash();
}

std::string Block::calculateHash() const
{
    std::stringstream ss;
    ss << m_index
        << m_timestamp
        << m_data
        << m_prevHash
        << m_nonce; // <--- include nonce

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(ss.str().c_str()),
        ss.str().size(),
        hash);

    std::stringstream hexStream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(hash[i]);
    }
    return hexStream.str();
}
