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

            Rule(u8string_view block_, u8string_view name_, u8string_view definition_)
              : block(block_), name(name_), definition(definition_)
            {}

            u8string_view block;
            u8string_view name;
            u8string_view definition;
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

        auto parseBlockDefinition() -> bool;
        auto parseBlockEnding() -> bool;

        auto completeBlock() -> void;

        auto parsingError(u8string_view expected_types, GenToken given_token) -> void;

        std::stack<Token> token_stack{};
        std::vector<std::pair<u8string_view, u8string_view>> directives{};
        std::vector<Rule> rules{};
        u8string_view current_block = u8"NONE";
        Tokenizer &tokenizer;
    };
}// namespace ccl::lex::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
