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
            u16 blockId{};
            u16 lastId{};
        };

        class Rule
        {
        public:
            string_view blockName;
            string_view name;
            string_view definition;
            u16 blockId{};
            u16 id{};

            Rule() = default;

            Rule(
                string_view block_name, BlockInfo &block_info, string_view rule_name,
                string_view rule_definition);
        };

    private:
        Vector<Rule> rules{};
        std::stack<Token> tokenStack{};
        Map<string_view, std::string> directives{};
        Map<string_view, BlockInfo> blocks{{"NONE", {0, 2}}};
        SpecialItems specialItems{};
        string_view currentBlock = "NONE";
        Tokenizer &tokenizer;
        size_t lastBlockId{1};

    public:
        explicit CcllParser(Tokenizer &input_tokenizer)
          : tokenizer{input_tokenizer}
        {}

        [[nodiscard]] auto getRules() const -> const Vector<Rule> &
        {
            return rules;
        }

        [[nodiscard]] auto getBlocks() const -> const Map<string_view, BlockInfo> &
        {
            return blocks;
        }

        [[nodiscard]] auto getDirectives() const -> const Map<string_view, std::string> &
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

        auto parsingError(string_view message, string_view suggestion = {}) -> void;
    };
}// namespace ccl::lex::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
