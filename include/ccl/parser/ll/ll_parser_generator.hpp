#ifndef CCL_PROJECT_LL_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LL_PARSER_GENERATOR_HPP

#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::ll
{
    class LlParserGenerator
    {
    public:
        std::unordered_map<TableEntry, const Rule *> table;
        FirstSetEvaluator firstSetEvaluator;
        FollowSetEvaluator followSetEvaluator;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;

        LlParserGenerator(
            SmallId start_symbol, const GrammarStorage &grammar_storage,
            std::function<std::string(SmallId)> id_to_string_converter);

    private:
        auto generateUsingRule(Symbol production, const Rule &rule) -> void;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_PARSER_GENERATOR_HPP */
