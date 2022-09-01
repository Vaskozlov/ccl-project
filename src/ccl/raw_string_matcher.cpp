#include <ccl/raw_string_matcher.hpp>

namespace ccl
{
    RawStringMatcher::RawStringMatcher(
        const std::initializer_list<std::pair<std::string, size_t>> &initial_data)
    {
        for (auto &&[string, value] : initial_data) {
            addString(string, value);
        }
    }

    auto RawStringMatcher::match(const string_view &string) const
        -> std::optional<std::pair<std::string, size_t>>
    {
        auto matching_str = getMatchingPart(string);

        if (map.contains(matching_str)) {
            return std::optional<std::pair<std::string, size_t>>(
                std::in_place, std::move(matching_str), map.at(matching_str));
        }

        return std::nullopt;
    }

    auto RawStringMatcher::matches(const string_view &string) const -> bool
    {
        auto matching_str = getMatchingPart(string);
        return map.contains(matching_str);
    }

    auto RawStringMatcher::reachable(const string_view &string, size_t level) const -> bool
    {
        return land(level != string.size(), level != char_levels.size());
    }

    auto RawStringMatcher::getMatchingPart(const string_view &string) const -> std::string
    {
        auto index = static_cast<size_t>(0);
        auto matching_str = std::string{};

        for (; reachable(string, index); ++index) {
            auto chr = string[index];
            const auto &level = char_levels[index];

            if (not level.at(std::bit_cast<u8>(chr))) {
                break;
            }

            matching_str.push_back(chr);
        }

        return matching_str;
    }

    auto RawStringMatcher::mapStringToLevels(string_view string) -> void
    {
        auto level = static_cast<size_t>(0);
        resizeLevels(string.size());

        for (auto chr : string) {
            char_levels[level++].set(std::bit_cast<u8>(chr), true);
        }
    }

    auto RawStringMatcher::resizeLevels(size_t new_size) -> void
    {
        if (char_levels.size() < new_size) {
            char_levels.resize(new_size);
        }
    }
}// namespace ccl
