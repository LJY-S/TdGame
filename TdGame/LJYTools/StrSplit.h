#pragma once

#include <vector>
#include <string_view>
#include <ranges>

std::vector<std::string_view> strSplit(const std::string_view& str, char delim)
{
    std::vector<std::string_view> ret;
    for (const auto& view : std::views::split(str, delim))
    {
        ret.emplace_back(view.data(), view.size());
    }
    return ret;
}