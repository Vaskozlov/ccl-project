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
        std::map<TableEntry, State> gotoTable;
        std::map<TableEntry, std::set<Action>> actionTable;
        std::vector<CanonicalCollection> canonicalCollection;
        std::map<TableEntry, State> transitions;
        std::function<std::string(Id)> idToStringConverter;
        const GrammarRulesStorage &grammarRules;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;
        std::map<Symbol, std::set<Symbol>> firstSet;

    public:
        explicit LrParserGenerator(
            const LrItem &start_item, Symbol epsilon_symbol,
            const GrammarRulesStorage &parser_rules,
            std::function<std::string(Id)> id_to_string_converter);

        [[nodiscard]] auto getGotoTable() -> std::map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getLrActionTable() const -> std::map<TableEntry, Action>;

        [[nodiscard]] auto getGlrActionTable() const -> std::map<TableEntry, std::vector<Action>>;

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        auto reduceAction(
            const Action &action,
            std::vector<State> &state_stack,
            std::vector<ast::UnNodePtr> &nodes_stack) const -> void;

        auto gotoFunction(const isl::UnorderedSet<LrItem> &items, Symbol symbol) const
            -> isl::UnorderedSet<LrItem>;

        auto doCanonicalCollectionConstructionIterationOnItem(
            Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
            std::vector<CanonicalCollection> &pending_collections) -> bool;

        auto doCanonicalCollectionConstructionIteration(
            Id &closure_id, std::set<Id> &marked_collections) -> bool;

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
