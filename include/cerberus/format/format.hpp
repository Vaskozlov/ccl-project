#ifndef CERBERUS_PROJECT_FORMAT_HPP
#define CERBERUS_PROJECT_FORMAT_HPP

#include <cerberus/const_string.hpp>
#include <cerberus/format/core/string_splitter.hpp>
#include <cerberus/format/dumpers/int.hpp>
#include <cerberus/format/dumpers/location.hpp>
#include <cerberus/format/dumpers/string.hpp>
#include <string>

namespace cerb::fmt
{
    template<ConstString String>
    class Formatter
    {
    public:
        CERBLIB_DECL auto get() -> std::u8string &
        {
            return formatted_string;
        }

        template<typename... Args>
        constexpr explicit Formatter(Args &&...args)
        {
            static_assert(sizeof...(Args) == string_blocks.size() - 1, "Wrong number of arguments");

            formatted_string.reserve(approximate_length);
            addBlock<0>(std::forward<Args>(args)...);
        }

    private:
        template<size_t Index>
        constexpr auto addBlock() -> void
        {
            if constexpr (isNotEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }
        }

        template<size_t Index, typename Arg, typename... Args>
        constexpr auto addBlock(Arg &&value, Args &&...values) -> void
        {
            if constexpr (isNotEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }

            dump(formatted_string, value);

            if constexpr (Index + 1 != string_blocks.size()) {
                addBlock<Index + 1>(std::forward<Args>(values)...);
            }
        }

        template<size_t Index>
        CERBLIB_DECL static auto isNotEmptyBlock() -> bool
        {
            return not string_blocks[Index].empty();
        }

        constexpr auto append(const u8string_view &str) -> void
        {
            if CERBLIB_RUNTIME_BRANCH {// G++-12 doesn't allow to append string at compile time
                formatted_string.append(static_cast<std::u8string_view>(str));
            } else {
                for (const auto &chr : str) {
                    formatted_string.push_back(chr);
                }
            }
        }

        constexpr static auto countApproximateLength() -> unsigned long
        {
            constexpr auto string_size = std::accumulate(
                string_blocks.begin(), string_blocks.end(), 0UL,
                [](auto acc, auto &block) { return acc + block.size(); });

            constexpr auto argument_count = string_blocks.size() - 1;
            constexpr auto arguments_approximate_length = argument_count * 8;

            return string_size + arguments_approximate_length;
        }

        constexpr static auto string_blocks = core::splitString<String>();
        constexpr static auto approximate_length = countApproximateLength();

        std::u8string formatted_string{};
    };

    template<ConstString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::u8string
    {
        auto formatter = Formatter<String>{ std::forward<Args>(args)... };
        return formatter.get();
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FORMAT_HPP */
