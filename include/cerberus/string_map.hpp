#ifndef CERBERUS_PROJECT_STRING_MAP_HPP
#define CERBERUS_PROJECT_STRING_MAP_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/typed_bitset.hpp>
#include <map>

namespace cerb
{
    template<CharacterLiteral CharT, std::integral Value>
    class StringMap
    {
    public:
        using Str = std::basic_string<CharT>;
        using Map = std::map<Str, Value>;
        using CharLevel = TypedBitset<CharT>;
        using CharLevels = boost::container::small_vector<CharLevel, 4>;// NOLINT

        struct Result
        {
            Result() = default;

            constexpr Result(bool success_, Str &&repr_, Value value_)
              : success(success_), value(value_), repr(std::move(repr_))
            {}

            bool success{};
            Value value{};
            Str repr{};
        };

        [[nodiscard]] auto size() const -> size_t
        {
            return map.size();
        }

        auto addString(Str &&string, Value value) -> void
        {
            mapStringToLevels(string);
            map.emplace(std::move(string), value);
        }

        auto addString(const Str &string, Value value) -> void
        {
            mapStringToLevels(string);
            map.emplace(string, value);
        }

        [[nodiscard]] auto matches(const Str &string) const -> Result
        {
            auto matching_str = getMatchingPart(string);

            if (map.contains(matching_str)) {
                return { true, std::move(matching_str), map.at(matching_str) };
            }

            return { false, {}, {} };
        }

        StringMap() = default;

        StringMap(const std::initializer_list<std::pair<Str, Value>> &initial_data)
        {
            for (auto &[string, value] : initial_data) {
                addString(string, value);
            }
        }

    private:
        [[nodiscard]] auto getMatchingPart(const Str &string) const -> Str
        {
            auto index = static_cast<size_t>(0);
            auto matching_str = std::basic_string<CharT>{};

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

        [[nodiscard]] auto reachable(const Str &string, size_t level) const -> bool
        {
            return land(level != string.size(), level != char_levels.size());
        }

        auto mapStringToLevels(const std::basic_string<CharT> &string) -> void
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
