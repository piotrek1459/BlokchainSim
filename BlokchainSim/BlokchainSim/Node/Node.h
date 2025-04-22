#ifndef NODE_H
#define NODE_H

#include "../Blockchain/Blockchain.h"

class Node
{
public:
    Node() = default;
    virtual ~Node() = default;

    // Retrieve the local blockchain
    Blockchain& getBlockchain() { return m_blockchain; }

    // Each node type can implement block creation differently
    virtual void createBlock(const std::string& data) = 0;

protected:
    Blockchain m_blockchain;
};

#endif // NODE_H
