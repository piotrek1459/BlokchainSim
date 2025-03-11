#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>

class Block
{
public:
    Block(unsigned int index, const std::string& data, const std::string& prevHash);

    unsigned int getIndex() const { return m_index; }
    std::time_t  getTimestamp() const { return m_timestamp; }
    std::string  getData() const { return m_data; }
    std::string  getPrevHash() const { return m_prevHash; }
    std::string  getHash() const { return m_hash; }

    std::string  calculateHash() const;

private:
    unsigned int m_index;
    std::time_t  m_timestamp;
    std::string  m_data;
    std::string  m_prevHash;
    std::string  m_hash;

    void generateHash();
};

#endif // BLOCK_H
