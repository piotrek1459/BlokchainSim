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

// ------------------------------------------------------------------
//  Strong validation ─ checks data integrity + link + optional PoW.
// ------------------------------------------------------------------
bool Blockchain::isChainValid() const
{
    constexpr bool CHECK_POW = true;                 // toggle if desired
    constexpr unsigned POW_DIFFICULTY = kDefaultDifficulty;

    const std::string powTarget(POW_DIFFICULTY, '0');

    for (std::size_t i = 0; i < m_chain.size(); ++i)
    {
        const Block& blk = m_chain[i];

        // 1) Hash must be internally consistent
        std::string recalculated = blk.calculateHash();
        if (recalculated != blk.getHash())
        {
            std::cerr << "[Invalid] Block " << i
                << " stored hash mismatch.\n";
            return false;
        }

        // 2) Optional PoW rule
        if (CHECK_POW && blk.getHash().substr(0, POW_DIFFICULTY) != powTarget && i > 0 && blk.getNonce() != 0)
        {
            std::cerr << "[Invalid] Block " << i
                << " fails POW difficulty.\n";
            return false;
        }

        // 3) prevHash pointer valid (skip genesis)
        if (i > 0)
        {
            const Block& prev = m_chain[i - 1];
            if (blk.getPrevHash() != prev.getHash())
            {
                std::cerr << "[Invalid] Block " << i
                    << " prevHash does not match block "
                    << (i - 1) << ".\n";
                return false;
            }
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


