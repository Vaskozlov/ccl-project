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
    public:
        struct TableEntry
        {
            State state{};
            Symbol lookAhead{};

            [[nodiscard]] auto operator<=>(const TableEntry &other) const noexcept
                -> std::strong_ordering = default;
        };

    private:
        isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> rules;
        isl::Set<CanonicalCollection> canonicalCollection;
        isl::Set<Symbol> allSymbols;
        isl::Set<Symbol> terminals;
        isl::Map<TableEntry, State> transitions;
        isl::Map<TableEntry, State> gotoTable;
        isl::Map<TableEntry, Action> actionTable;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilon;
        isl::Map<Symbol, isl::Set<Symbol>> firstSet;

    public:
        explicit LrParser(
            const LrItem &start_item, Symbol epsilon_symbol, isl::Set<Symbol> grammar_symbols,
            isl::Set<Symbol> terminal_symbols,
            isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> parser_rules);

        auto
            parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const -> std::unique_ptr<ast::Node>;

        [[nodiscard]] auto getGotoTable() const noexcept -> const isl::Map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getActionTable() const noexcept -> const isl::Map<TableEntry, Action> &
        {
            return actionTable;
        }

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return terminals.contains(symbol);
        }

        auto reduceAction(
            const Action &action,
            isl::Vector<State> &state_stack,
            isl::Vector<std::unique_ptr<ast::Node>> &nodes_stack) const -> void;

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

    template<typename T>
    struct TableEntryPrintHelper
    {
        const typename LrParser::TableEntry &entry;
    };
}// namespace ccl::parser

template<typename T>
class fmt::formatter<ccl::parser::TableEntryPrintHelper<T>>
  : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto
        format(ccl::parser::TableEntryPrintHelper<T> entry_print_helper, FmtContext &ctx) const
    {
        const auto &entry = entry_print_helper.entry;

        return fmt::format_to(
            ctx.out(), "{}-{}", entry.state,
            ccl::lexer::lexerEnumToString(isl::as<T>(entry.lookAhead)));
    }
};

#endif /* CCL_PROJECT_LR_PARSER_HPP */
