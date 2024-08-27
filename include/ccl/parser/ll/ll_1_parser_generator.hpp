#ifndef CCL_PROJECT_LL_1_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LL_1_PARSER_GENERATOR_HPP

#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::ll
{
    using Ll1Table = ankerl::unordered_dense::map<TableEntry, const Rule *>;

    class Ll1ParserGenerator
    {
    private:
        Ll1Table table;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;

    public:
        Ll1ParserGenerator(
            const GrammarStorage &grammar_storage,
            const std::function<std::string(SmallId)> &id_to_string_converter);

        [[nodiscard]] auto createLl1Table() const -> const Ll1Table &
        {
            return table;
        }

    private:
        auto insertIntoTable(TableEntry entry, const Rule *rule) -> void;

        auto generateUsingRule(Symbol production, const Rule &rule) -> void;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_1_PARSER_GENERATOR_HPP */
