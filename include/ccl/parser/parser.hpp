#ifndef CCL_PROJECT_PARSER_HPP
#define CCL_PROJECT_PARSER_HPP

#include <atomic>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parsing_rules.hpp>
#include <functional>

#define CCL_PARSING_RULE(Type, RuleConstructor, ...)                                               \
    ccl::parser::ParsingRule(Type, CCL_STR(Type), RuleConstructor, {__VA_ARGS__})

namespace ccl::parser
{
    struct Parser
    {
        using Tokenizer = typename lex::LexicalAnalyzer::Tokenizer;
        using FollowSet = SmallVector<const ParsingRule *>;

        CCL_ENUM(// NOLINTNEXTLINE
            TerminalMatchResult, size_t, FALSE, TRUE, CONTINUE);

        struct CCL_TRIVIAL_ABI MismatchResult
        {
            Optional<Id> stack_version;
            Optional<Id> rule_version;
        };

        explicit Parser(const ParsingRules &parsing_rules_, Tokenizer &tokenizer_);

        auto parse() -> void;

    private:
        auto parse(Stack &stack, const FollowSet &follow_set) -> bool;

        auto fullMatchCase(Stack &stack, const FollowSet &follow_set, const ParsingRule &rule)
            -> bool;

        auto nonTerminalCase(
            Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result)
            -> bool;

        auto terminalCase(
            Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result,
            const ParsingRule &rule) -> TerminalMatchResult;

        auto parseWithNewFollowSet(
            Id expected_type, Stack &stack,
            size_t passing_elements = std::numeric_limits<size_t>::max(),
            const std::function<bool(const ParsingRule &)> &pred = [](const ParsingRule &) {
                return true;
            }) -> bool;

        [[nodiscard]] auto formFollowSet(
            Id expected_type, const std::function<bool(const ParsingRule &)> &pred) const
            -> FollowSet;

        [[nodiscard]] static auto mismatch(const Stack &stack, const ParsingRule &rule)
            -> MismatchResult;

        [[nodiscard]] static auto fullyMatched(const MismatchResult &mismatch_result) -> bool;

        [[nodiscard]] auto isTerminal(const MismatchResult &mismatch_result) -> bool;

        [[nodiscard]] auto isNonTerminal(const MismatchResult &mismatch_result) const -> bool;

        [[maybe_unused]] static auto reduce(Stack &stack, const ParsingRule &rule) -> void;

        auto pushNewToken(Stack &stack) -> void;

        auto throwError(Stack &stack, const FollowSet &follow_set) -> void;

        Tokenizer &tokenizer;
        ExceptionHandler &exception_handler{tokenizer.getHandler()};
        const Set<Id> &terminals{};
        const Set<Id> &nonTerminals{};
        const Map<Id, SmallVector<ParsingRule>> &parsingRules;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER_HPP */
