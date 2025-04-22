#ifndef FULLNODE_H
#define FULLNODE_H

#include "Node.h"

class FullNode : public Node
{
public:
    explicit FullNode(Blockchain& chain) : Node(chain) {}

    // Just make a block and add it (no proof-of-work)
    void createBlock(const std::string& data) override {
        Block b = m_blockchain.createNextBlock(data);
        m_blockchain.addBlock(b);
    }
};

#endif // FULLNODE_H
