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

void Blockchain::addBlock(const Block& b)
{
    m_chain.push_back(b);
}

void Blockchain::addBlock(const std::string& data)
{
    Block candidate = createNextBlock(data);
    Block mined = mineBlock(candidate, kDefaultDifficulty);
    m_chain.push_back(mined);
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

Block Blockchain::createNextBlock(const std::string& data) const
{
    unsigned int newIndex = static_cast<unsigned int>(m_chain.size());
    return Block(newIndex, data, m_chain.back().getHash());
}

Block Blockchain::mineBlock(Block block, unsigned int difficulty) const
{
    std::string target(difficulty, '0');
    unsigned int nonce = 0;

    while (true) {
        block.setNonce(nonce++);
        if (block.calculateHash().substr(0, difficulty) == target) {
            block.setHash(block.calculateHash());
            return block;
        }
    }
}


