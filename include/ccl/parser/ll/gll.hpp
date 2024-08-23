#ifndef CCL_PROJECT_GLL_HPP
#define CCL_PROJECT_GLL_HPP

#include <ccl/parser/ll/ll_parser_generator.hpp>
#include <ccl/parser/parsing_result.hpp>

namespace ccl::parser {
    class GllParser {
    private:
        ll::GllTable table;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage&storage;
        SmallId grammarGoalSymbol;

    public:
        GllParser(
            SmallId start_symbol, const GrammarStorage&grammar_storage,
            const std::function<std::string(SmallId)>&id_to_string_converter);

        auto parse(lexer::Tokenizer&tokenizer) -> AmbiguousParsingResult;
    };
} // namespace ccl::parser

#endif /* CCL_PROJECT_GLL_HPP */
