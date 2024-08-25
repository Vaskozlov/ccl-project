#ifndef CCL_PROJECT_GLL_PARSER_GENERATOR_HPP
#define CCL_PROJECT_GLL_PARSER_GENERATOR_HPP

#include <ccl/parser/first_set.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::ll
{
    using GllTable = ankerl::unordered_dense::map<TableEntry, std::vector<const Rule *>>;

    class GllParserGenerator
    {
    private:
        GllTable table;
        FirstSetEvaluator firstSetEvaluator;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;

    public:
        GllParserGenerator(
            const GrammarStorage &grammar_storage,
            const std::function<std::string(SmallId)> &id_to_string_converter);

        [[nodiscard]] auto createGllTable() const -> const GllTable &
        {
            return table;
        }

    private:
        auto insertIntoTable(TableEntry entry, const Rule *rule) -> void;

        auto generateUsingRule(Symbol production, const Rule &rule) -> void;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_GLL_PARSER_GENERATOR_HPP */
