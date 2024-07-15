#ifndef CCL_PROJECT_LR_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LR_PARSER_GENERATOR_HPP

#include <ccl/parser/action.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/canonical_collection.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser
{
    class LrParserGenerator
    {
    private:
        isl::Map<TableEntry, State> gotoTable;
        isl::Map<TableEntry, Action> actionTable;
        isl::Set<CanonicalCollection> canonicalCollection;
        isl::Map<TableEntry, State> transitions;
        const GrammarRulesStorage &grammarRules;
        const isl::Set<Symbol> &allSymbols;
        const isl::Set<Symbol> &terminalSymbols;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;
        isl::Map<Symbol, isl::Set<Symbol>> firstSet;

    public:
        explicit LrParserGenerator(
            const LrItem &start_item, Symbol epsilon_symbol,
            const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminal_symbols,
            const GrammarRulesStorage &parser_rules);

        [[nodiscard]] auto getGotoTable() -> isl::Map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getActionTable() -> isl::Map<TableEntry, Action> &
        {
            return actionTable;
        }

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return terminalSymbols.contains(symbol);
        }

        auto reduceAction(
            const Action &action,
            isl::Vector<State> &state_stack,
            isl::Vector<ast::NodePtr> &nodes_stack) const -> void;

        auto gotoFunction(const isl::Set<LrItem> &items, Symbol symbol) const -> isl::Set<LrItem>;

        auto doCanonicalCollectionConstructionIterationOnItem(
            Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
            isl::Set<CanonicalCollection> &pending_collections) -> bool;

        auto doCanonicalCollectionConstructionIteration(
            Id &closure_id, isl::Set<Id> &marked_collections) -> bool;

        auto constructCanonicalCollection(const LrItem &start_item) -> void;

        auto doClosureComputationIteration(isl::Set<LrItem> &s, const LrItem &item) const -> bool;

        auto computeClosure(isl::Set<LrItem> s) const -> isl::Set<LrItem>;

        auto fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void;

        auto fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item) -> void;

        auto fillGotoTableEntry(const CanonicalCollection &cc, Symbol symbol) -> void;

        auto fillTables() -> void;

        template<typename... Ts>
        auto insertIntoActionTable(TableEntry entry, Ts &&...args) -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_GENERATOR_HPP */
