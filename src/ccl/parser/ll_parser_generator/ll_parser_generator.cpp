#include <ccl/parser/ll/ll_parser_generator.hpp>

namespace ccl::parser::ll
{
    LlParserGenerator::LlParserGenerator(
        SmallId start_symbol,
        const GrammarRulesStorage &grammar_storage,
        std::function<std::string(SmallId)>
            id_to_string_converter)
      : firstSetEvaluator{grammar_storage.getEpsilon(), grammar_storage}
      , followSetEvaluator(
            start_symbol,
            0,
            grammar_storage.getEpsilon(),
            grammar_storage,
            firstSetEvaluator.getFirstSet())
      , idToStringConverter{std::move(id_to_string_converter)}
      , storage{grammar_storage}
    {
        for (auto &[key, rule] : storage.rulesIterator()) {
            generateUsingRule(key, rule);
        }
    }

    auto LlParserGenerator::generateUsingRule(
        ccl::parser::Symbol production,
        const ccl::parser::Rule &rule) -> void
    {
        for (auto symbol : rule) {
            if (!storage.isTerminal(symbol)) {
                continue;
            }

            // TODO: add error state for table
        }

        fmt::println("{}", std::views::transform(rule, idToStringConverter));
        const auto &rule_first_set =
            firstSetEvaluator.getFirstSetOfRules().at(std::addressof(rule));

        auto set = rule_first_set;

        if (set.contains(storage.getEpsilon())) {
            set.insert_range(followSetEvaluator.getFollowSet().at(production));
        }

        fmt::println("{}", std::views::transform(set, idToStringConverter));

        for (auto symbol : set) {
            if (!storage.isTerminal(symbol) || symbol == storage.getEpsilon()) {
                continue;
            }

            table.try_emplace({production, symbol}, std::addressof(rule));
            fmt::println(
                "[{},{}] = {}",
                idToStringConverter(production),
                idToStringConverter(symbol),
                std::views::transform(rule, idToStringConverter));
        }
    }
}// namespace ccl::parser::ll
