#include "Blockchain/Blockchain.h"
#include <gtest/gtest.h>

TEST(ChainValidation, DetectsTampering)
{
    Blockchain chain;
    chain.addBlock("Alice->Bob:10");
    ASSERT_TRUE(chain.isChainValid());

    // --- manual tamper ---
    auto& blk = const_cast<Block&>(chain.getChain().back());
    blk.setHash("1234");   // corrupt stored hash

    EXPECT_FALSE(chain.isChainValid());
}
