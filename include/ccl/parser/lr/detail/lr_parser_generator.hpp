#ifndef CCL_PROJECT_LR_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LR_PARSER_GENERATOR_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/canonical_collection.hpp>
#include <ccl/parser/lr/detail/table_entry.hpp>

namespace ccl::parser
{
    class LrParserGenerator
    {
    private:
        isl::Map<TableEntry, State> gotoTable;
        isl::Map<TableEntry, isl::Set<Action>> actionTable;
        isl::Vector<CanonicalCollection> canonicalCollection;
        isl::Map<TableEntry, State> transitions;
        const GrammarRulesStorage &grammarRules;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;
        isl::Map<Symbol, isl::Set<Symbol>> firstSet;

    public:
        explicit LrParserGenerator(
            const LrItem &start_item, Symbol epsilon_symbol,
            const GrammarRulesStorage &parser_rules);

        [[nodiscard]] auto getGotoTable() -> isl::Map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getLrActionTable() const -> isl::Map<TableEntry, Action>;

        [[nodiscard]] auto getGlrActionTable() const -> isl::Map<TableEntry, isl::Vector<Action>>;

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        auto reduceAction(
            const Action &action,
            isl::Vector<State> &state_stack,
            isl::Vector<ast::UnNodePtr> &nodes_stack) const -> void;

        auto gotoFunction(const isl::UnorderedSet<LrItem> &items, Symbol symbol) const
            -> isl::UnorderedSet<LrItem>;

        auto doCanonicalCollectionConstructionIterationOnItem(
            Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
            isl::Vector<CanonicalCollection> &pending_collections) -> bool;

        auto doCanonicalCollectionConstructionIteration(
            Id &closure_id, isl::Set<Id> &marked_collections) -> bool;

        auto constructCanonicalCollection(const LrItem &start_item) -> void;

        auto doClosureComputationIteration(isl::UnorderedSet<LrItem> &s, const LrItem &item) const
            -> bool;

        auto computeClosure(isl::UnorderedSet<LrItem> s) const -> isl::UnorderedSet<LrItem>;

        auto fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void;

        auto fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item) -> void;

        auto fillGotoTableEntry(const CanonicalCollection &cc, Symbol symbol) -> void;

        auto fillTables() -> void;

        template<typename... Ts>
        auto insertIntoActionTable(TableEntry entry, Ts &&...args) -> void;

        auto insertIntoGotoTable(TableEntry entry, State state) -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_GENERATOR_HPP */
