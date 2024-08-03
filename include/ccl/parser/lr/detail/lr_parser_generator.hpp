#ifndef CCL_PROJECT_LR_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LR_PARSER_GENERATOR_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/canonical_collection.hpp>
#include <ccl/parser/lr/detail/table_entry.hpp>
#include <ccl/runtime.hpp>
#include <list>
#include <queue>

namespace ccl::parser
{
    class LrParserGenerator
    {
    private:
        struct GotoResult
        {
            std::vector<LrItem> items;
            Symbol symbol;
            SmallId canonicalCollectionId;
        };

        enum class PipePopStatus : u8
        {
            SUCCEED,
            PIPE_EMPTY,
            PIPE_CLOSED
        };

        using CanonicalCollectionIterator = typename std::list<CanonicalCollection>::iterator;

        std::unordered_map<TableEntry, State> gotoTable;
        std::unordered_map<TableEntry, std::set<Action>> actionTable;

        std::unordered_map<TableEntry, State> transitions;
        std::list<CanonicalCollection> canonicalCollection;
        CanonicalCollectionIterator lastPolledCanonicalCollection;

        std::function<std::string(SmallId)> idToStringConverter;

        const GrammarRulesStorage &grammarRules;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;

        std::unordered_map<Symbol, std::unordered_set<Symbol>> firstSet;
        std::map<std::vector<LrItem>, std::vector<LrItem>> closureComputationOnItemsCache;
        std::unordered_map<LrItem, std::vector<LrItem>> closureComputationCache;

        runtime::Pipe<GotoResult, 1024> pipe;

    public:
        explicit LrParserGenerator(
            const LrItem &start_item, Symbol epsilon_symbol,
            const GrammarRulesStorage &parser_rules,
            std::function<std::string(SmallId)> id_to_string_converter);

        [[nodiscard]] auto getGotoTable() -> std::unordered_map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getLrActionTable() const -> std::unordered_map<TableEntry, Action>;

        [[nodiscard]] auto
            getGlrActionTable() const -> std::unordered_map<TableEntry, std::vector<Action>>;

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        auto pushIntoPipe(GotoResult &value) -> void;

        [[nodiscard]] auto tryPopFromPipe(GotoResult &value) noexcept -> PipePopStatus;

        auto reduceAction(
            const Action &action,
            std::vector<State> &state_stack,
            std::vector<ast::UnNodePtr> &nodes_stack) const -> void;

        auto moveCollectionItemsOverSymbol(
            const CanonicalCollection &canonical_collection,
            Symbol symbol) -> std::vector<LrItem>;

        auto moveCollectionItemsOverRemainingSymbols(
            const CanonicalCollection &canonical_collection) -> void;

        auto pollCanonicalCollection() -> isl::Task<>;

        auto fillCanonicalCollection(isl::thread::IdGenerator &closure_id) -> bool;

        auto constructCanonicalCollection(const LrItem &start_item) -> void;

        auto computeClosure(const LrItem &item) -> const std::vector<LrItem> &;

        auto computeClosureOnItems(std::vector<LrItem> s) -> const std::vector<LrItem> &;

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
