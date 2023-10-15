#ifndef CCL_PROJECT_CCLL_PARSER_HPP
#define CCL_PROJECT_CCLL_PARSER_HPP

#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>
#include <stack>

namespace ccl::lexer::parser
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
            isl::string_view name;
            isl::string_view definition;
            u16 blockId{};
            u16 id{};

            Rule() = default;

            Rule(
                BlockInfo &block_info, isl::string_view rule_name,
                isl::string_view rule_definition);
        };

    private:
        std::vector<Rule> rules{};
        std::stack<Token> tokenStack{};
        std::map<isl::string_view, std::string> directives{};
        std::map<isl::string_view, BlockInfo> blocks{{"NONE", {0, 2}}};
        SpecialItems specialItems{};
        isl::string_view currentBlock = "NONE";
        Tokenizer &tokenizer;
        size_t lastBlockId{1};

    public:
        explicit CcllParser(Tokenizer &input_tokenizer);

        [[nodiscard]] auto getRules() const CCL_LIFETIMEBOUND->const std::vector<Rule> &
        {
            return rules;
        }

        [[nodiscard]] auto
            getBlocks() const CCL_LIFETIMEBOUND->const std::map<isl::string_view, BlockInfo> &
        {
            return blocks;
        }

        [[nodiscard]] auto
            getDirectives() const CCL_LIFETIMEBOUND->const std::map<isl::string_view, std::string> &
        {
            return directives;
        }

        auto parse() -> bool;

    private:
        auto completeRule(const Token &token) -> void;
        auto completeDirective(const Token &token) -> void;
        auto completeGroup(const Token &token) -> void;

        auto checkRule(const Token &token) -> void;

        auto parsingError(isl::string_view message, isl::string_view suggestion = {}) -> void;
    };
}// namespace ccl::lexer::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
