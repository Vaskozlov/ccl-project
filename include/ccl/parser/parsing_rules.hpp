#ifndef CCL_PROJECT_PARSING_RULES_HPP
#define CCL_PROJECT_PARSING_RULES_HPP

#include <ccl/parser/parsing_rule.hpp>

namespace ccl::parser
{
    class ParsingRules
    {
        friend Parser;

    public:
        using PrecedenceTable = UnorderedMap<RuleId, size_t>;

        ParsingRules(PrecedenceTable precedence_table_, InitializerList<ParsingRule> rules_);

    private:
        auto initializeRules(InitializerList<ParsingRule> initializer_list) -> void;
        auto detectTerminals() -> void;

        auto fixConflicts() -> void;
        auto fixConflict(ParsingRule &rule, const ParsingRule &other_rule) -> void;

        auto fixUsingPrecedence(ParsingRule &rule, RuleId first_mismatch, RuleId second_mismatch) const
            -> void;

        auto checkThereAreNoCloseNonTerminals(const ParsingRule &rule) -> void;

        PrecedenceTable precedence_table{};
        Set<RuleId> terminals{};
        Set<RuleId> non_terminals{};
        Map<RuleId, SmallVector<ParsingRule, 4>> parsing_rules{};
        bool errors_in_rules{ false };
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSING_RULES_HPP */
