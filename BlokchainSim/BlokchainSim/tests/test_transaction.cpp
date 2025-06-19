#include "Blockchain/Transaction.h"
#include <gtest/gtest.h>

TEST(TransactionParse, AcceptsValidString)
{
    const auto tx = Transaction::parse("Alice->Bob:99.5");
    EXPECT_EQ(tx.from, "Alice");
    EXPECT_EQ(tx.to, "Bob");
    EXPECT_DOUBLE_EQ(tx.amount, 99.5);
}

TEST(TransactionParse, RejectsInvalidString)
{
    EXPECT_THROW(Transaction::parse("BadInputNoDelimiter"),
        std::invalid_argument);
    EXPECT_THROW(Transaction::parse("Alice->Bob:notnumber"),
        std::invalid_argument);
}
