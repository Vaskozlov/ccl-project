#ifndef CCL_PROJECT_PARSING_RULES_HPP
#define CCL_PROJECT_PARSING_RULES_HPP

#include <ccl/parser/parsing_rule.hpp>

namespace ccl::parser
{
    class ParsingRules
    {
        friend Parser;

    public:
        using PrecedenceTable = UnorderedMap<Id, size_t>;

        ParsingRules(PrecedenceTable precedence_table_, InitializerList<ParsingRule> rules_);

    private:
        auto initializeRules(InitializerList<ParsingRule> initializer_list) -> void;
        auto detectTerminals() -> void;

        auto fixConflicts() -> void;
        auto fixConflict(ParsingRule &rule, const ParsingRule &other_rule) -> void;

        auto fixUsingPrecedence(ParsingRule &rule, Id first_mismatch, Id second_mismatch) const
            -> void;

        auto checkThereAreNoCloseNonTerminals(const ParsingRule &rule) -> void;

        PrecedenceTable precedence_table{};
        Set<Id> terminals{};
        Set<Id> non_terminals{};
        Map<Id, SmallVector<ParsingRule>> parsing_rules{};
        bool errors_in_rules{false};
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSING_RULES_HPP */
