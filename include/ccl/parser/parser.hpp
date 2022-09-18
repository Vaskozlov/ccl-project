#ifndef CCL_PROJECT_PARSER_HPP
#define CCL_PROJECT_PARSER_HPP

#include <ccl/ccl.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/node.hpp>
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace ccl::parser
{
    struct StackGetter
    {
        explicit StackGetter(std::deque<NodePtr> &stack_) : stack(stack_)
        {}

        auto pop() -> std::unique_ptr<Node>
        {
            auto node = std::move(stack.front());
            stack.pop_front();
            return node;
        }

        std::deque<NodePtr> &stack;
    };

    struct ParsingRule
    {
        using iterator = typename std::deque<NodePtr>::iterator;

        NodePtr (*constructor)(StackGetter) = nullptr;
        std::vector<size_t> ids_to_construct{};
        std::unordered_set<size_t> can_not_be_followed{};
    };

    class ParsingRules
    {
    public:
        using Tokenizer = lex::LexicalAnalyzer::Tokenizer;

        struct Parser
        {
            Parser(const ParsingRules &parser_, Tokenizer &tokenizer_)
              : rules(parser_.rules), tokenizer(tokenizer_),
                token_to_string(parser_.token_to_string)
            {}

            auto parse() -> void;

        private:
            auto scanWithRules() -> void;
            auto applyRule(const ParsingRule &rule) -> bool;

            [[nodiscard]] auto matchesRule(const ParsingRule &rule) const -> bool;
            [[nodiscard]] auto shouldSkipRule(const ParsingRule &rule, size_t future_id) const
                -> bool;

            [[nodiscard]] auto tokenToString(size_t id) const -> std::string;

            template<typename T, typename Pred>
            auto toString(const T &container, Pred &&function) const -> std::string;

            std::deque<NodePtr> stack{};
            std::vector<size_t> follow_set{};
            const std::unordered_map<size_t, std::vector<ParsingRule>> &rules;
            Tokenizer &tokenizer;
            std::string_view (*token_to_string)(size_t){ nullptr };
        };

        ParsingRules(
            std::vector<ParsingRule>
                rules_,
            std::unordered_map<size_t, size_t>
                precedence_table_,
            std::string_view (*token_to_string_)(size_t) = nullptr);

        [[nodiscard]] auto getParser(Tokenizer &tokenizer) const -> Parser
        {
            return { *this, tokenizer };
        }

    private:
        auto needToReduce(size_t last_id) const -> bool;

        auto reduceTableConflicts() -> void;
        auto reduceConflict(size_t entry_id, ParsingRule &rule) -> void;
        auto completeReduction(size_t first_id, size_t second_id, ParsingRule &rule) -> void;

        auto initializeTable(std::vector<ParsingRule> &table_rules) -> void;

        [[nodiscard]] auto tokenToString(size_t id) const -> std::string;

        template<typename T, typename Pred>
        auto toString(const T &container, Pred &&function) const -> std::string;

        std::unordered_map<size_t, std::vector<ParsingRule>> rules;
        std::unordered_map<size_t, size_t> precedence_table;
        std::string_view (*token_to_string)(size_t){ nullptr };
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER_HPP */
