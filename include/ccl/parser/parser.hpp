#ifndef CCL_PROJECT_PARSER_HPP
#define CCL_PROJECT_PARSER_HPP

#include <atomic>
#include <boost/container/flat_map.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/node.hpp>
#include <unordered_map>
#include <unordered_set>

#define CCL_PARSING_RULE(Type, RuleConstructor, ...)                                               \
    ccl::parser::ParsingRule(Type, CCL_STR(Type), RuleConstructor, { __VA_ARGS__ })

namespace ccl::parser
{
    CCL_ENUM(RuleOnStackResult, size_t, NO_MATCH, FULL_MATCH, PARTIAL_MATCH);// NOLINT
    CCL_ENUM(ParsingRuleType, size_t, EOI, BAD_TOKEN, ROOT);                 // NOLINT

    using RuleType = size_t;

    struct Parser;
    class ParsingRules;

    struct ParsingStack
    {
        explicit ParsingStack(std::vector<NodePtr> &stack_) : stack(stack_)
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

    struct ParsingRule
    {
        friend ParsingRules;

        ParsingRule() = default;

        ParsingRule(
            RuleType type_, std::string_view name_, NodePtr (*rule_constructor_)(ParsingStack),
            const std::initializer_list<size_t> &ids_to_constructs_)
          : ids_to_constructs(ids_to_constructs_), name(name_), rule_constructor(rule_constructor_),
            type(type_)
        {}

        std::vector<size_t> ids_to_constructs{};
        std::unordered_set<size_t> ids_that_forbid_construction{};
        std::string_view name{ "set name for ccl::parser::ParsingRule" };
        NodePtr (*rule_constructor)(ParsingStack) = nullptr;
        RuleType type{};
        size_t uuid{};

    private:
        static std::atomic<size_t> uuid_counter;// NOLINT
    };

    class ParsingRules
    {
        friend Parser;

    public:
        ParsingRules(const std::initializer_list<ParsingRule> &rules_);

    private:
        auto initializeRules(const std::initializer_list<ParsingRule> &initializer_list) -> void;

        std::unordered_map<RuleType, std::vector<ParsingRule>> rules{};
    };

    struct Parser
    {
        using Tokenizer = typename lex::LexicalAnalyzer::Tokenizer;
        using Flatmap = boost::container::flat_map<size_t, std::vector<ParsingRule>>;

        explicit Parser(const ParsingRules &parsing_rules_, Tokenizer &tokenizer_)
          : tokenizer(tokenizer_), parsing_rules(parsing_rules_.rules)
        {}

        auto parse() -> void;

    private:
        auto parse(const Flatmap &follow_set) -> bool;

        auto parseWithNewFollowSet(const Flatmap &follow_set, const ParsingRule &rule) -> bool;

        [[nodiscard]] auto isRuleOnStack(const ParsingRule &rule) const -> RuleOnStackResult;

        auto reduce(const ParsingRule &rule) -> void;

        auto pushNewToken() -> void;

        std::vector<NodePtr> stack{};
        Tokenizer &tokenizer;
        const std::unordered_map<RuleType, std::vector<ParsingRule>> &parsing_rules;
    };
}// namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::ParsingRule> : fmt::formatter<std::string_view>
{
    auto format(const ccl::parser::ParsingRule &rule, format_context &ctx) const
    {
        return formatter<std::string_view>::format(rule.name, ctx);
    }
};

#endif /* CCL_PROJECT_PARSER_HPP */
