#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "../Block/Block.h"
#include "concepts.hpp"

class Blockchain
{
public:
    Blockchain();

    Block createNextBlock(const std::string& data) const;
    void addBlock(const std::string& data);
    void addBlock(const Block& block);
    bool isChainValid() const;
    const std::vector<Block>& getChain() const { return m_chain; }
    void clear() { m_chain.clear(); }
    Block mineBlock(Block block, unsigned int difficulty) const;
    template<Hashable H>
    static bool verifyBlock(const H& blk, const std::string& prevHash)
    {
        return blk.calculateHash() == blk.getHash()
            && blk.getPrevHash() == prevHash;
    }


private:
    std::vector<Block> m_chain;
    Block createGenesisBlock();
    static constexpr unsigned int kDefaultDifficulty = 2;
};



#endif // BLOCKCHAIN_H
