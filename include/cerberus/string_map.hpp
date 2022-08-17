#ifndef CERBERUS_PROJECT_STRING_MAP_HPP
#define CERBERUS_PROJECT_STRING_MAP_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/utf_set.hpp>
#include <map>
#include <optional>

namespace cerb
{
    template<std::integral Value>
    class StringMap
    {
    public:
        using Map = std::map<std::u8string, Value>;
        using CharLevel = UtfSet;
        using CharLevels = boost::container::small_vector<CharLevel, 4>;// NOLINT

        [[nodiscard]] auto size() const noexcept -> size_t
        {
            return map.size();
        }

        template<typename T>
            requires std::is_convertible_v<T, std::u8string>
        auto addString(T &&string, Value value) -> void
        {
            mapStringToLevels(string);
            map.emplace(std::forward<T>(string), value);
        }

        [[nodiscard]] auto match(const u8string_view &string) const
            -> std::optional<std::pair<std::u8string, Value>>
        {
            auto matching_str = getMatchingPart(string);

            if (map.contains(matching_str)) {
                return std::make_pair(std::move(matching_str), map.at(matching_str));
            }

            return std::nullopt;
        }

        [[nodiscard]] auto matches(const u8string_view &string) const -> bool
        {
            auto matching_str = getMatchingPart(string);
            return map.contains(matching_str);
        }

        StringMap() = default;

        StringMap(const std::initializer_list<std::pair<std::u8string, Value>> &initial_data)
        {
            for (auto &[string, value] : initial_data) {
                addString(string, value);
            }
        }

    private:
        [[nodiscard]] auto getMatchingPart(const u8string_view &string) const -> std::u8string
        {
            auto index = static_cast<size_t>(0);
            auto matching_str = std::u8string{};

            for (; reachable(string, index); ++index) {
                auto chr = string[index];
                const auto &level = char_levels[index];

                if (not level.at(chr)) {
                    break;
                }

                matching_str.push_back(chr);
            }

            return matching_str;
        }

        [[nodiscard]] auto reachable(const u8string_view &string, size_t level) const -> bool
        {
            return land(level != string.size(), level != char_levels.size());
        }

        auto mapStringToLevels(const std::u8string &string) -> void
        {
            auto level = static_cast<size_t>(0);
            resizeLevels(string.size());

            for (auto chr : string) {
                char_levels[level++].set(chr, true);
            }
        }

        auto resizeLevels(size_t new_size) -> void
        {
            if (char_levels.size() < new_size) {
                char_levels.resize(new_size);
            }
        }

        Map map{};
        CharLevels char_levels = CharLevels(4);
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_STRING_MAP_HPP */
