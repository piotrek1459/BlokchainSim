#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>

class Block
{
public:
    Block(unsigned int index,
        const std::string& data,
        const std::string& prevHash);

	//From disk constructor
    Block(unsigned idx,
        std::time_t ts,
        std::string data,
        std::string prev,
        unsigned nonce,
        std::string storedHash)
        : m_index(idx),
        m_timestamp(ts),
        m_data(std::move(data)),
        m_prevHash(std::move(prev)),
        m_nonce(nonce),
        m_hash(std::move(storedHash))
    { /* no generateHash() */}


    unsigned int getIndex() const { return m_index; }
    std::time_t  getTimestamp() const { return m_timestamp; }
    std::string  getData() const { return m_data; }
    std::string  getPrevHash() const { return m_prevHash; }
    std::string  getHash() const { return m_hash; }

    unsigned int getNonce() const { return m_nonce; }
    void         setNonce(unsigned int nonce) { m_nonce = nonce; }
    void         setHash(const std::string& newHash) { m_hash = newHash; }
    void         setTimestamp(std::time_t ts) { m_timestamp = ts; }

    std::string calculateHash() const;  // used repeatedly during mining

private:
    unsigned int m_index;
    std::time_t  m_timestamp;
    std::string  m_data;
    std::string  m_prevHash;
    std::string  m_hash;

    unsigned int m_nonce;  

    void generateHash();   
};

#endif // BLOCK_H
