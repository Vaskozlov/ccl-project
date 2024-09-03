#include <ccl/text/remove_escaping.hpp>
#include <ccl/text/text_iterator.hpp>
#include <isl/utf8.hpp>

namespace ccl::text
{
    auto removeEscaping(
        const isl::string_view input,
        const std::span<isl::Pair<char32_t, char32_t>>
            additional_escaping_symbols) -> std::string
    {
        auto result = std::string{};
        auto text_iterator = TextIterator{input};

        while (true) {
            auto [is_escaping, character] =
                text_iterator.nextCharWithEscapingSymbols(additional_escaping_symbols);

            if (!is_escaping && isEoF(character)) {
                break;
            }

            isl::utf8::appendUtf32ToUtf8Container(result, character);
        }

        return result;
    }
}// namespace ccl::text