#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "../Block/Block.h"

class Blockchain
{
public:
    Blockchain();

    void addBlock(const std::string& data);
    bool isChainValid() const;

    const std::vector<Block>& getChain() const { return m_chain; }

private:
    std::vector<Block> m_chain;

    Block createGenesisBlock();
};

#endif // BLOCKCHAIN_H
