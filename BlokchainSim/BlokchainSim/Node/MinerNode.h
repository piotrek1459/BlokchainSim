#ifndef MINERNODE_H
#define MINERNODE_H

#include "Node.h"
#include <future>
#include <atomic>

class MinerNode : public Node
{
public:
    MinerNode(Blockchain& chain, unsigned int difficulty = 2)
        : Node(chain),
        m_difficulty(difficulty),
        m_mining(false) {}

    void setDifficulty(unsigned int d) { m_difficulty = d; }

    void createBlock(const std::string& data) override {
        Block b = m_blockchain.createNextBlock(data);

        // Launch mining in a background thread (async)
        m_mining.store(true);
        std::future<Block> fut = std::async(std::launch::async,
            &MinerNode::mineBlock,
            this,
            b);

        // Wait for the mining to finish and add the result
        Block minedBlock = fut.get();
        m_mining.store(false);
        m_blockchain.addBlock(minedBlock);
    }

private:
    unsigned int m_difficulty;
    std::atomic<bool> m_mining;

    Block mineBlock(Block block) {
        std::string target(m_difficulty, '0');
        unsigned int nonce = 0;
        while (true) {
            block.setNonce(nonce++);
            std::string hashValue = block.calculateHash();
            if (hashValue.substr(0, m_difficulty) == target) {
                block.setHash(hashValue);
                return block;
            }
        }
    }
};

#endif // MINERNODE_H
