#include "Blockchain.h"

Blockchain::Blockchain()
{
    // Insert the “genesis” block
    m_chain.push_back(createGenesisBlock());
}

Block Blockchain::createGenesisBlock()
{
    return Block(0, "Genesis Block", "0");
}

void Blockchain::addBlock(const std::string& data)
{
    unsigned int newIndex = static_cast<unsigned int>(m_chain.size());
    std::string prevHash = m_chain.back().getHash();

    Block newBlock(newIndex, data, prevHash);
    m_chain.push_back(newBlock);
}

bool Blockchain::isChainValid() const
{
    for (size_t i = 1; i < m_chain.size(); ++i) {
        const Block& curr = m_chain[i];
        const Block& prev = m_chain[i - 1];

        // Recalculate the hash and compare
        if (curr.calculateHash() != curr.getHash()) {
            return false;
        }
        // Check previous hash pointer
        if (curr.getPrevHash() != prev.getHash()) {
            return false;
        }
    }
    return true;
}
