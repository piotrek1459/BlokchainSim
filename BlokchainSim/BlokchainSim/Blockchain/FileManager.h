#pragma once
#include <filesystem>
#include <fstream>
#include "../nlohmann/json.hpp"   // convenient single‑header JSON (add to project)

#include "Blockchain.h"

class FileManager
{
public:
    static std::filesystem::path defaultPath()
    {
        return std::filesystem::path{ std::getenv("USERPROFILE") ? std::getenv("USERPROFILE") : "." }
            / "blockchain"
            / "chain.json";
    }

    static void save(const Blockchain& chain,
        const std::filesystem::path& p = defaultPath())
    {
        std::filesystem::create_directories(p.parent_path());

        nlohmann::json j;
        for (const auto& blk : chain.getChain())
        {
            j.push_back({
                {"index",      blk.getIndex()},
                {"timestamp",  blk.getTimestamp()},
                {"nonce",      blk.getNonce()},
                {"data",       blk.getData()},
                {"prevHash",   blk.getPrevHash()},
                {"hash",       blk.getHash()}
                });
        }
        std::ofstream(p) << j.dump(2);
    }
};
