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

    static Blockchain load(const std::filesystem::path& p = defaultPath())
    {
        Blockchain chain;                       // already has a genesis block
        if (!std::filesystem::exists(p))
            return chain;                       // file missing → OK

        std::ifstream f(p);
        if (f.peek() == std::ifstream::traits_type::eof())
            return chain;                       // file empty → keep genesis

        nlohmann::json j;  
        f >> j;
        if (j.empty())
            return chain;                       // parsed but no blocks → keep genesis

        chain.clear();                          // only clear if we really have data
        for (const auto& e : j)
        {
            Block blk(e["index"],
                e["timestamp"],
                e["data"],
                e["prevHash"],
                e["nonce"],
                e["hash"]);
            chain.addBlock(blk);
        }
        return chain;
    }

};
