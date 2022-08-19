#include <cerberus/raw_string_matcher.hpp>

namespace cerb
{
    auto RawStringMatcher::match(const u8string_view &string) const
        -> std::optional<std::pair<std::u8string, size_t>>
    {
        auto matching_str = getMatchingPart(string);

        if (map.contains(matching_str)) {
            return std::make_pair(std::move(matching_str), map.at(matching_str));
        }

        return std::nullopt;
    }


    auto RawStringMatcher::matches(const u8string_view &string) const -> bool
    {
        auto matching_str = getMatchingPart(string);
        return map.contains(matching_str);
    }

    auto RawStringMatcher::reachable(const u8string_view &string, size_t level) const -> bool
    {
        return land(level != string.size(), level != char_levels.size());
    }

    auto RawStringMatcher::getMatchingPart(const u8string_view &string) const -> std::u8string
    {
        auto index = static_cast<size_t>(0);
        auto matching_str = std::u8string{};

        for (; reachable(string, index); ++index) {
            auto chr = string[index];
            auto &level = char_levels[index];

            if (not level.at(chr)) {
                break;
            }

            matching_str.push_back(chr);
        }

        return matching_str;
    }

    auto RawStringMatcher::mapStringToLevels(const std::u8string &string) -> void
    {
        auto level = static_cast<size_t>(0);
        resizeLevels(string.size());

        for (auto chr : string) {
            char_levels[level++].set(chr, true);
        }
    }

    auto RawStringMatcher::resizeLevels(size_t new_size) -> void
    {
        if (char_levels.size() < new_size) {
            char_levels.resize(new_size);
        }
    }
}// namespace cerb
