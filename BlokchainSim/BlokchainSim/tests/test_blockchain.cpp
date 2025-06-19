#include "Blockchain/Blockchain.h"
#include <gtest/gtest.h>

TEST(ChainTest, ValidAfterAddingBlock)
{
    Blockchain chain;
    chain.addBlock("Alice->Bob:10");

    EXPECT_TRUE(chain.isChainValid());
    EXPECT_EQ(chain.getChain().size(), 2);  // genesis + 1
}
