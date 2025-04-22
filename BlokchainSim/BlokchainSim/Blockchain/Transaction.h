#pragma once
#include <string>
#include <regex>
#include <stdexcept>

class Transaction
{
public:
    std::string from;
    std::string to;
    double      amount{ 0.0 };

    // Parse "Alice->Bob:50"
    static Transaction parse(const std::string& str)
    {
        static const std::regex re(R"((\w+)\s*->\s*(\w+)\s*:\s*([0-9]+(\.[0-9]+)?))");
        std::smatch m;
        if (!std::regex_match(str, m, re))
            throw std::invalid_argument("Invalid tx format: " + str);

        Transaction tx;
        tx.from = m[1];
        tx.to = m[2];
        tx.amount = std::stod(m[3]);
        return tx;
    }
};
