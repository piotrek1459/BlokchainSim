#include "Blockchain/Blockchain.h"
#include "Node/MinerNode.h"
#include <gtest/gtest.h>

TEST(ProofOfWork, HashMeetsDifficulty)
{
    Blockchain chain;
    MinerNode miner(chain, /*difficulty=*/2);

    miner.createBlock("Tx");               // mines synchronously
    const auto& last = chain.getChain().back();

    EXPECT_TRUE(last.getHash().rfind("00", 0) == 0);  // starts with "00"
    EXPECT_NE(last.getNonce(), 0u);                   // nonce was changed
}
