#ifndef CCL_PROJECT_RAW_STRING_MATCHER_HPP
#define CCL_PROJECT_RAW_STRING_MATCHER_HPP

#include <boost/container/small_vector.hpp>
#include <ccl/string_view.hpp>
#include <ccl/utf_set.hpp>
#include <map>
#include <optional>

namespace ccl
{
    class RawStringMatcher
    {
    public:
        using CharLevel = UtfSet;
        using Map = std::map<std::string, size_t>;
        using CharLevels = boost::container::small_vector<CharLevel, 4>;// NOLINT

        [[nodiscard]] auto size() const noexcept -> size_t
        {
            return map.size();
        }

        CCL_PERFECT_FORWARDING(T, std::string)
        auto addString(T &&string, size_t value) -> void
        {
            mapStringToLevels(string);
            map.emplace(std::forward<T>(string), value);
        }

        [[nodiscard]] auto match(const string_view &string) const
            -> std::optional<std::pair<std::string, size_t>>;

        [[nodiscard]] auto matches(const string_view &string) const -> bool;

        RawStringMatcher() = default;

        RawStringMatcher(const std::initializer_list<std::pair<std::string, size_t>> &initial_data);

    private:
        [[nodiscard]] auto reachable(const string_view &string, size_t level) const -> bool;
        [[nodiscard]] auto getMatchingPart(const string_view &string) const -> std::string;

        auto resizeLevels(size_t new_size) -> void;
        auto mapStringToLevels(string_view string) -> void;

        Map map{};
        CharLevels char_levels = CharLevels(4);
    };
}// namespace ccl

#endif /* CCL_PROJECT_RAW_STRING_MATCHER_HPP */
