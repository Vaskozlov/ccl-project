#ifndef CERBERUS_PROJECT_RAW_STRING_MATCHER_HPP
#define CERBERUS_PROJECT_RAW_STRING_MATCHER_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/utf_set.hpp>
#include <map>
#include <optional>

namespace cerb
{
    class RawStringMatcher
    {
    public:
        using CharLevel = UtfSet;
        using Map = std::map<std::u8string, size_t>;
        using CharLevels = boost::container::small_vector<CharLevel, 4>;// NOLINT

        [[nodiscard]] auto size() const noexcept -> size_t
        {
            return map.size();
        }

        CERBLIB_PERFECT_FORWARDING(T, std::u8string)
        auto addString(T &&string, size_t value) -> void
        {
            mapStringToLevels(string);
            map.emplace(std::forward<T>(string), value);
        }

        [[nodiscard]] auto match(const u8string_view &string) const
            -> std::optional<std::pair<std::u8string, size_t>>;

        [[nodiscard]] auto matches(const u8string_view &string) const -> bool;

        RawStringMatcher() = default;

        RawStringMatcher(
            const std::initializer_list<std::pair<std::u8string, size_t>> &initial_data)
        {
            for (auto &&[string, value] : initial_data) {
                addString(string, value);
            }
        }

    private:
        [[nodiscard]] auto reachable(const u8string_view &string, size_t level) const -> bool;
        [[nodiscard]] auto getMatchingPart(const u8string_view &string) const -> std::u8string;

        auto resizeLevels(size_t new_size) -> void;
        auto mapStringToLevels(const std::u8string &string) -> void;

        Map map{};
        CharLevels char_levels = CharLevels(4);
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_RAW_STRING_MATCHER_HPP */
