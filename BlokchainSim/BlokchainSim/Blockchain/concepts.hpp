#pragma once
#include <string>
#include <concepts>

// Any type that has calculateHash() returning std::string
template<typename T>
concept Hashable = requires(T t)
{
    { t.calculateHash() } -> std::same_as<std::string>;
};
