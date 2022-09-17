#ifndef CCL_PROJECT_CCLL_PARSER_HPP
#define CCL_PROJECT_CCLL_PARSER_HPP

#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <map>
#include <stack>

namespace ccl::lex::parser
{
    class CcllParser
    {
    public:
        using Tokenizer = typename LexicalAnalyzer::Tokenizer;

        struct BlockInfo
        {
            u16 block_id{};
            u16 last_id{};
        };

        struct Rule
        {
            Rule() = default;

            Rule(
                string_view block_name_, BlockInfo &block_info_, string_view name_,
                string_view definition_)
              : block_name(block_name_), name(name_), definition(definition_),
                block_id(block_info_.block_id), id(block_info_.last_id++)
            {}

            string_view block_name;
            string_view name;
            string_view definition;
            u16 block_id{};
            u16 id{};
        };

        explicit CcllParser(Tokenizer &tokenizer_) : tokenizer(tokenizer_)
        {}

        [[nodiscard]] auto getRules() const -> const std::vector<Rule> &
        {
            return rules;
        }

        [[nodiscard]] auto getBlocks() const -> const std::map<string_view, BlockInfo> &
        {
            return blocks;
        }

        [[nodiscard]] auto getDirectives() const -> const std::map<string_view, std::string> &
        {
            return directives;
        }

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

        auto expectRuleEnd() -> void;

        auto recoverFromError() -> void;

        auto parsingError(
            string_view expected_types, GenToken given_token, string_view suggestion = {}) -> void;

    private:
        std::vector<Rule> rules{};
        std::stack<Token> token_stack{};
        std::map<string_view, std::string> directives{};
        std::map<string_view, BlockInfo> blocks{ { "NONE", { 0, 2 } } };
        SpecialItems special_items{};
        string_view current_block = "NONE";
        Tokenizer &tokenizer;
        size_t last_block_id{ 1 };
    };
}// namespace ccl::lex::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
