#pragma once
#include <ranges>
#include <numeric>
#include "Blockchain.h"

class ChainStats
{
public:
    struct Result {
        double averageNonce{};
        std::time_t earliest{};
        std::time_t latest{};
    };

    static Result compute(const Blockchain& chain)
    {
        auto& c = chain.getChain();
        auto nonceRange = c | std::views::transform([](const Block& b) { return b.getNonce(); });
        auto tsRange = c | std::views::transform([](const Block& b) { return b.getTimestamp(); });

        Result res;
        res.averageNonce = std::accumulate(nonceRange.begin(), nonceRange.end(), 0u)
            / static_cast<double>(c.size());

        res.earliest = *std::ranges::min_element(tsRange);
        res.latest = *std::ranges::max_element(tsRange);
        return res;
    }
};
