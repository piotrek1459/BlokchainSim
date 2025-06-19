#include "Block/Block.h"
#include <gtest/gtest.h>

TEST(BlockTest, HashChangesWithNonce)
{
    Block b(0, "data", "0");
    auto original = b.getHash();

    b.setNonce(b.getNonce() + 1);
    b.setHash(b.calculateHash());

    EXPECT_NE(original, b.getHash());
}
