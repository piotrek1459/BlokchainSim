#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "../Block/Block.h"

class Blockchain
{
public:
    Blockchain();

    Block createNextBlock(const std::string& data) const;
    void addBlock(const std::string& data);
    void addBlock(const Block& block);
    bool isChainValid() const;
    const std::vector<Block>& getChain() const { return m_chain; }


private:
    std::vector<Block> m_chain;
    Block mineBlock(Block block, unsigned int difficulty) const;
    Block createGenesisBlock();
    static constexpr unsigned int kDefaultDifficulty = 2;
};

#endif // BLOCKCHAIN_H
