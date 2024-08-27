#ifndef CCL_PROJECT_LR_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LR_PARSER_GENERATOR_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/canonical_collection.hpp>
#include <ccl/parser/table_entry.hpp>
#include <ccl/runtime.hpp>
#include <list>

namespace ccl::parser::lr
{
    using LrGotoTable = ankerl::unordered_dense::map<TableEntry, State>;
    using Lr1ActionTable = ankerl::unordered_dense::map<TableEntry, Action>;
    using GlrActionTable = ankerl::unordered_dense::map<TableEntry, std::vector<Action>>;

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

        using CanonicalCollectionIterator = std::list<CanonicalCollection>::iterator;

        LrGotoTable gotoTable;
        GlrActionTable actionTable;

        ankerl::unordered_dense::map<TableEntry, State> transitions;
        std::list<CanonicalCollection> canonicalCollection;
        CanonicalCollectionIterator lastPolledCanonicalCollection;

        std::function<std::string(SmallId)> idToStringConverter;

        const GrammarStorage &grammarRules;
        Symbol goalProduction;
        Symbol endOfInput;
        Symbol epsilonSymbol;

        ankerl::unordered_dense::map<std::vector<LrItem>, std::vector<LrItem>>
            closureComputationOnItemsCache;

        ankerl::unordered_dense::map<LrItem, std::vector<LrItem>> closureComputationCache;

        runtime::Pipe<GotoResult, 256> pipe;

    public:
        explicit LrParserGenerator(
            const LrItem &start_item, Symbol epsilon_symbol, const GrammarStorage &parser_rules,
            std::function<std::string(SmallId)> id_to_string_converter);

        [[nodiscard]] auto getGotoTable() -> auto &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getLrActionTable() const -> Lr1ActionTable;

        [[nodiscard]] auto getGlrActionTable() const -> const GlrActionTable &;

    private:
        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        auto pushIntoPipe(GotoResult &value) -> void;

        [[nodiscard]] auto tryPopFromPipe(GotoResult &value) noexcept -> PipePopStatus;

        auto moveCollectionItemsOverSymbol(
            const CanonicalCollection &canonical_collection,
            Symbol symbol) -> std::vector<LrItem>;

        auto moveCollectionItemsOverRemainingSymbols(
            const CanonicalCollection &canonical_collection) -> void;

        auto generateCanonicalCollection() -> isl::Task<>;

        auto fillCanonicalCollection(isl::IdGenerator<SmallId> &closure_id) -> bool;

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
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_PARSER_GENERATOR_HPP */
