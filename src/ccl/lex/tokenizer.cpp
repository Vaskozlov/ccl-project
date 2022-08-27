#include <ccl/lex/lexical_analyzer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    // NOLINTNEXTLINE recursive function
    auto LexicalAnalyzer::Tokenizer::yield() -> Token
    {
        text_iterator.skipComments();

        if (text_iterator.isEnd()) {
            return Token{ { text_iterator }, ReservedTokenType::EOI, u8"$"s };
        }

        if (auto special_token = lexical_analyzer.shared.getSpecialToken(text_iterator);
            special_token.has_value()) {
            return *special_token;
        }

        for (auto &&item : lexical_analyzer.items) {
            auto scan_result = item.scan(text_iterator, Token{ text_iterator, item.getId() }, true);

            if (scan_result.has_value() && not scan_result->second.getRepr().empty()) {
                auto &[iterator, token] = *scan_result;
                text_iterator = std::move(iterator);
                return token;
            }
        }

        if (isLayout(text_iterator.getNextCarriageValue())) {
            text_iterator.next();
            return yield();
        }

        text_iterator.throwException<LexicalAnalysisException>(u8"unrecognizable token");
        return constructBadToken();
    }


    auto LexicalAnalyzer::Tokenizer::constructBadToken() -> Token
    {
        auto token = Token{ text_iterator, ReservedTokenType::BAD_TOKEN };

        while (not lexical_analyzer.shared.isNextCharNotForScanning(text_iterator)) {
            text_iterator.next();
        }

        token.setEnd(text_iterator.getRemainingAsCarriage());
        return token;
    }
}// namespace ccl::lex
