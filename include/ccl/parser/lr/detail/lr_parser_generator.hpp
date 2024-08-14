#ifndef CCL_PROJECT_LR_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LR_PARSER_GENERATOR_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/canonical_collection.hpp>
#include <ccl/parser/table_entry.hpp>
#include <ccl/runtime.hpp>
#include <list>

namespace ccl::parser
{
    class LrParserGenerator
    {
    private:
        struct GotoResult
        {
            std::vector<GrammarSlot> items;
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

        ankerl::unordered_dense::map<TableEntry, State> gotoTable;
        ankerl::unordered_dense::map<TableEntry, std::set<Action>> actionTable;

        ankerl::unordered_dense::map<TableEntry, State> transitions;
        std::list<CanonicalCollection> canonicalCollection;
        CanonicalCollectionIterator lastPolledCanonicalCollection;

        std::function<std::string(SmallId)> idToStringConverter;

        const GrammarStorage &grammarRules;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;

        std::unordered_map<Symbol, std::unordered_set<Symbol>> firstSet;

        ankerl::unordered_dense::map<std::vector<GrammarSlot>, std::vector<GrammarSlot>>
            closureComputationOnItemsCache;

        ankerl::unordered_dense::map<GrammarSlot, std::vector<GrammarSlot>> closureComputationCache;

        runtime::Pipe<GotoResult, 256> pipe;

    public:
        explicit LrParserGenerator(
            const GrammarSlot &start_item, Symbol epsilon_symbol,
            const GrammarStorage &parser_rules,
            std::function<std::string(SmallId)> id_to_string_converter);

        [[nodiscard]] auto getGotoTable() -> auto &
        {
            return gotoTable;
        }

        [[nodiscard]] auto
            getLrActionTable() const -> ankerl::unordered_dense::map<TableEntry, Action>;

        [[nodiscard]] auto getGlrActionTable() const
            -> ankerl::unordered_dense::map<TableEntry, std::vector<Action>>;

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        auto pushIntoPipe(GotoResult &value) -> void;

        [[nodiscard]] auto tryPopFromPipe(GotoResult &value) noexcept -> PipePopStatus;

        auto moveCollectionItemsOverSymbol(
            const CanonicalCollection &canonical_collection,
            Symbol symbol) -> std::vector<GrammarSlot>;

        auto moveCollectionItemsOverRemainingSymbols(
            const CanonicalCollection &canonical_collection) -> void;

        auto generateCanonicalCollection() -> isl::Task<>;

        auto fillCanonicalCollection(isl::thread::IdGenerator<SmallId> &closure_id) -> bool;

        auto constructCanonicalCollection(const GrammarSlot &start_item) -> void;

        auto computeClosure(const GrammarSlot &item) -> const std::vector<GrammarSlot> &;

        auto computeClosureOnItems(std::vector<GrammarSlot> s) -> const std::vector<GrammarSlot> &;

        auto fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void;

        auto fillActionTableEntry(const CanonicalCollection &cc, const GrammarSlot &item) -> void;

        auto fillGotoTableEntry(const CanonicalCollection &cc, Symbol symbol) -> void;

        auto fillTables() -> void;

        template<typename... Ts>
        auto insertIntoActionTable(TableEntry entry, Ts &&...args) -> void;

        auto insertIntoGotoTable(TableEntry entry, State state) -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_GENERATOR_HPP */
