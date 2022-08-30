#ifndef CCL_PROJECT_CCLL_PARSER_HPP
#define CCL_PROJECT_CCLL_PARSER_HPP

#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <stack>

namespace ccl::lex::parser
{
    class CcllParser
    {
    public:
        using Tokenizer = typename LexicalAnalyzer::Tokenizer;

        struct Rule
        {
            Rule() = default;

            Rule(string_view block_, string_view name_, string_view definition_)
              : block(block_), name(name_), definition(definition_)
            {}

            string_view block;
            string_view name;
            string_view definition;
            size_t id{};
        };

        explicit CcllParser(Tokenizer &tokenizer_) : tokenizer(tokenizer_)
        {}

        auto parse() -> bool;

    private:
        auto parseDeclaration() -> bool;
        auto parseRuleDeclaration() -> bool;
        auto parseDirectiveDeclaration() -> bool;

        auto completeRuleDeclaration() -> void;
        auto completeDirectiveDeclaration() -> void;

        auto checkRule(text::TextIterator &rule) -> void;

        auto parseBlockDefinition() -> bool;
        auto parseBlockEnding() -> bool;

        auto completeBlock() -> void;

        auto parsingError(
            string_view expected_types, GenToken given_token, string_view suggestion = {}) -> void;

        std::stack<Token> token_stack{};
        std::vector<std::pair<string_view, string_view>> directives{};
        std::vector<Rule> rules{};
        AnalysisShared analysis_shared{};
        string_view current_block = "NONE";
        Tokenizer &tokenizer;
    };
}// namespace ccl::lex::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
