#include "Blockchain/FileManager.h"
#include <gtest/gtest.h>
#include <filesystem>

TEST(FileManager, SaveAndLoadRoundTrip)
{
    Blockchain original;
    original.addBlock("Alice->Bob:1");

    const auto tmp = std::filesystem::temp_directory_path() / "chain_test.json";
    FileManager::save(original, tmp);

    Blockchain loaded = FileManager::load(tmp);
    EXPECT_TRUE(loaded.isChainValid());
    EXPECT_EQ(loaded.getChain().size(), original.getChain().size());

    std::filesystem::remove(tmp);   // clean up
}
