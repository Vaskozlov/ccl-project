#ifndef CCL_PROJECT_PARSER2_HPP
#define CCL_PROJECT_PARSER2_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/node.hpp>
#include <unordered_map>
#include <unordered_set>

namespace ccl::parser
{
    struct StackGetter2
    {
        explicit StackGetter2(std::vector<NodePtr> &stack_) : stack(stack_)
        {}

        auto pop() -> NodePtr
        {
            auto node = std::move(stack.back());
            stack.pop_back();
            return node;
        }

    private:
        std::vector<NodePtr> &stack;
    };

    struct RuleForParsing
    {
        NodePtr (*rule_constructor)(StackGetter2){ nullptr };
        std::vector<size_t> ids_to_construct{};
        std::unordered_set<size_t> forbidden_future_ids{};
        size_t id{};
    };

    class TokenFormatter
    {
    public:
        explicit TokenFormatter(std::string_view (*token_to_string_)(size_t) = nullptr)
          : token_to_string(token_to_string_)
        {}

        [[nodiscard]] auto formatToken(size_t token) const -> std::string
        {
            if (token_to_string == nullptr) {
                return std::to_string(token);
            }

            return std::string(token_to_string(token));
        }

        [[nodiscard]] auto formatContainerOfTokens(const auto &container, auto &&pred) const
            -> std::string
        {
            if (container.begin() == container.end()) {
                return "[]";
            }

            auto result = std::string("[");
            auto begin = container.begin();

            result.append(formatToken(pred(*begin)));

            for (++begin; begin != container.end(); ++begin) {
                result.append(", ");
                result.append(formatToken(pred(*begin)));
            }

            result.push_back(']');
            return result;
        }

        [[nodiscard]] auto formatContainerOfTokens(const auto &container) const -> std::string
        {
            return formatContainerOfTokens(container, defaultFormatter);
        }

    private:
        static auto defaultFormatter(size_t token) -> size_t
        {
            return token;
        }

        std::string_view (*token_to_string)(size_t);
    };

    struct Parser2;

    class ParsingRules2
    {
    public:
        using PrecedenceTable = std::unordered_map<size_t, size_t>;

        friend Parser2;

        ParsingRules2(
            const std::initializer_list<RuleForParsing> &rules_,
            PrecedenceTable precedence_table_,
            std::string_view (*token_to_string_)(size_t) = nullptr);

    private:
        auto initializeTable(const std::initializer_list<RuleForParsing> &rules) -> void;

        auto fixConflicts() -> void;

        auto fixShiftReduceConflict(RuleForParsing &rule) -> void;
        static auto fixShiftReduceConflict(RuleForParsing &rule, RuleForParsing &similar_rule)
            -> void;

        auto fixReduceReduceConflict(RuleForParsing &rule) -> void;
        auto fixReduceReduceConflict(RuleForParsing &rule, RuleForParsing &conflicted_rule) -> void;

        std::unordered_map<size_t, std::vector<RuleForParsing>> first_set{};
        PrecedenceTable precedence_table{};
        TokenFormatter token_formatter;
    };

    struct Parser2
    {
        using Tokenizer = typename lex::LexicalAnalyzer::Tokenizer;

        Parser2(const ParsingRules2 &rules_, Tokenizer &tokenizer_)
          : tokenizer(tokenizer_), first_set(rules_.first_set),
            token_formatter(rules_.token_formatter)
        {}

        auto parser() -> void;

    private:
        auto refillSet() -> void;

        auto iteration() -> void;

        [[nodiscard]] auto applyRule(const RuleForParsing &rule) -> bool;

        [[nodiscard]] auto matchesRule(const RuleForParsing &rule) -> bool;

        [[nodiscard]] auto needToSkip(const RuleForParsing &rule, size_t next_id) const -> bool;

        std::unordered_map<size_t, std::vector<RuleForParsing>> current_set{};
        std::vector<NodePtr> stack{};
        Tokenizer &tokenizer;
        const std::unordered_map<size_t, std::vector<RuleForParsing>> &first_set{};
        TokenFormatter token_formatter;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER2_HPP */
