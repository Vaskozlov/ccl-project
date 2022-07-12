#ifndef CERBERUS_PROJECT_FORMAT_HPP
#define CERBERUS_PROJECT_FORMAT_HPP

#include <cerberus/format/converters/int.hpp>
#include <cerberus/format/converters/location.hpp>
#include <cerberus/format/converters/string.hpp>
#include <cerberus/format/core/string_splitter.hpp>
#include <cerberus/template_string.hpp>
#include <string>

namespace cerb::fmt
{
    template<TemplateString String>
    class Formatter
    {
    private:
        using CharT = decltype(String.zeroChar());

    public:
        CERBLIB_DECL auto get() -> std::basic_string<CharT> &
        {
            return formatted_string;
        }

        template<typename... Args>
        constexpr explicit Formatter(Args &&...args)
        {
            static_assert(sizeof...(Args) == string_blocks.size() - 1, "Wrong number of arguments");
            addBlock<0>(std::forward<Args>(args)...);
        }

    private:
        template<size_t Index>
        constexpr auto addBlock() -> void
        {
            if constexpr (not isEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }
        }

        template<size_t Index, typename Arg, typename... Args>
        constexpr auto addBlock(Arg &&value, Args &&...values) -> void
        {
            if constexpr (not isEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }

            convert(formatted_string, value);

            if constexpr (Index + 1 != string_blocks.size()) {
                addBlock<Index + 1>(std::forward<Args>(values)...);
            }
        }

        template<size_t Index>
        CERBLIB_DECL static auto isEmptyBlock() -> bool
        {
            return string_blocks[Index].empty();
        }

        constexpr auto append(const BasicStringView<CharT> &str) -> void
        {
            if CERBLIB_RUNTIME_BRANCH {// G++-12 doesn't allow to append string at compile time
                formatted_string.append(str.begin(), str.end());
            } else {
                for (auto &chr : str) {
                    formatted_string.push_back(chr);
                }
            }
        }

        constexpr static auto string_blocks = core::splitString<String>();

        std::basic_string<CharT> formatted_string{};
    };

    template<TemplateString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<decltype(String.zeroChar())>
    {
        auto formatter = Formatter<String>{ std::forward<Args>(args)... };
        return formatter.get();
    }

    template<CharacterLiteral To, TemplateString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<To>
    {
        auto formatter = Formatter<String.template convert<To>()>{ std::forward<Args>(args)... };
        return formatter.get();
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FORMAT_HPP */
