#ifndef CCL_PROJECT_LR_PARSER_HPP
#define CCL_PROJECT_LR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/action.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/canonical_collection.hpp>

namespace ccl::parser
{
    class LrParser
    {
    private:
        struct TableEntry
        {
            Id state{};
            Id lookAhead{};

            [[nodiscard]] auto operator<=>(const TableEntry &other) const noexcept
                -> std::strong_ordering = default;
        };

        isl::Map<Id, isl::Vector<isl::Vector<Id>>> rules;
        isl::Set<CanonicalCollection> canonicalCollection;
        isl::Set<Id> allSymbols;
        isl::Set<Id> terminals;
        isl::Map<TableEntry, Id> transitions;
        isl::Map<TableEntry, Id> gotoTable;
        isl::Map<TableEntry, Action> actionTable;
        std::size_t goalProduction;
        std::size_t endOfInput;
        std::size_t epsilon;
        isl::Map<Id, isl::Set<Id>> firstSet;

    public:
        explicit LrParser(
            const LrItem &start_item, Id epsilon_symbol, isl::Set<Id> grammar_symbols,
            isl::Set<Id> terminal_symbols, isl::Map<Id, isl::Vector<isl::Vector<Id>>> parser_rules);

        auto parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> std::unique_ptr<ast::Node>;

    private:
        [[nodiscard]] auto isTerminal(std::size_t symbol) const noexcept -> bool
        {
            return terminals.contains(symbol);
        }

        auto gotoFunction(const isl::Set<LrItem> &items, std::size_t product) -> isl::Set<LrItem>;

        auto doCanonicalCollectionConstructionIterationOnItem(
            Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
            isl::Set<CanonicalCollection> &pending_collections) -> bool;

        auto doCanonicalCollectionConstructionIteration(
            Id &closure_id, isl::Set<Id> &marked_collections) -> bool;

        auto constructCanonicalCollection(const LrItem &start_item) -> void;

        auto doClosureComputationIteration(isl::Set<LrItem> &s, const LrItem &item) -> bool;

        auto computeClosure(isl::Set<LrItem> s) -> isl::Set<LrItem>;

        auto fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void;

        auto fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item) -> void;

        auto fillGotoTableEntry(const CanonicalCollection &cc, std::size_t product) -> void;

        auto fillTables() -> void;

        auto checkForConflictsInActionTable(const TableEntry &entry, ParsingAction parsingAction)
            -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_HPP */
