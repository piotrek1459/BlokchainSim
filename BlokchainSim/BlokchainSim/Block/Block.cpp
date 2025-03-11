#include "Block.h"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>  // or any other hashing library

Block::Block(unsigned int index,
    const std::string& data,
    const std::string& prevHash)
    : m_index(index),
    m_timestamp(std::time(nullptr)),
    m_data(data),
    m_prevHash(prevHash)
{
    generateHash();
}

std::string Block::calculateHash() const
{
    std::stringstream ss;
    ss << m_index << m_timestamp << m_data << m_prevHash;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(ss.str().c_str()),
        ss.str().size(), hash);

    std::stringstream hexStream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(hash[i]);
    }
    return hexStream.str();
}

void Block::generateHash()
{
    m_hash = calculateHash();
}
