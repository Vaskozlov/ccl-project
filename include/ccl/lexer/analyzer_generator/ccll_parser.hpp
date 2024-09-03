#ifndef CCL_PROJECT_CCLL_PARSER_HPP
#define CCL_PROJECT_CCLL_PARSER_HPP

#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>
#include <isl/id_generator.hpp>

namespace ccl::lexer::parser
{
    class CcllParser
    {
    public:
        using Tokenizer = LexicalAnalyzer::Tokenizer;

        class Rule
        {
        public:
            isl::string_view name;
            isl::string_view definition;
            SmallId id{};

            Rule() = default;

            Rule(SmallId rule_id, isl::string_view rule_name, isl::string_view rule_definition);
        };

    private:
        class CompareTokensByRepr
        {
        public:
            [[nodiscard]] auto operator()(const Token &lhs, const Token &rhs) const noexcept -> bool
            {
                return lhs.getRepr() < rhs.getRepr();
            }
        };

        std::vector<Rule> rules;
        std::vector<Token> tokenStack;
        std::map<Token, isl::string_view, CompareTokensByRepr> directives;
        AnyPlaceItems anyPlaceItems;
        Tokenizer &tokenizer;
        isl::IdGenerator<SmallId> idGenerator{ReservedTokenMaxValue + 1};

    public:
        explicit CcllParser(Tokenizer &input_tokenizer);

        [[nodiscard]] auto getRules() const CCL_LIFETIMEBOUND -> const auto &
        {
            return rules;
        }

        [[nodiscard]] auto getDirectives() const CCL_LIFETIMEBOUND -> const auto &
        {
            return directives;
        }

        auto parse() -> bool;

    private:
        auto completeRule(const Token &token) -> void;
        auto completeDirective(const Token &token) -> void;

        auto checkRule(const Token &token) const -> void;

        auto parsingError(isl::string_view message, isl::string_view suggestion = {}) const -> void;
    };
}// namespace ccl::lexer::parser

#endif /* CCL_PROJECT_CCLL_PARSER_HPP */
