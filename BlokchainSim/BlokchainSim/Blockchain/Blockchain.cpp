#include "Blockchain.h"
#include "Transaction.h"
#include <iostream>
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
    try {
        Transaction tx = Transaction::parse(data);
        std::cout << "Parsed TX  " << tx.from
            << " -> " << tx.to
            << " : " << tx.amount << '\n';
    }
    catch (const std::exception&) {
        std::cout << "Raw data   " << data << '\n';
    }

    Block candidate = createNextBlock(data);
    Block mined = mineBlock(candidate, kDefaultDifficulty);
    m_chain.push_back(mined);
}

bool Blockchain::isChainValid() const
{
    for (size_t i = 1; i < m_chain.size(); ++i) {
        if (!verifyBlock(m_chain[i], m_chain[i - 1].getHash()))
            return false;
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


