#include <ccl/parser/ll/ll_1_parser_generator.hpp>

namespace ccl::parser::ll
{
    Ll1ParserGenerator::Ll1ParserGenerator(
        SmallId start_symbol,
        const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : firstSetEvaluator{grammar_storage.getEpsilon(), grammar_storage}
      , followSetEvaluator(
            start_symbol,
            std::to_underlying(lexer::ReservedTokenType::EOI),
            grammar_storage.getEpsilon(),
            grammar_storage,
            firstSetEvaluator.getFirstSet())
      , idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
    {
        for (auto &[key, rule] : storage.rulesIterator()) {
            generateUsingRule(key, rule);
        }
    }

    auto Ll1ParserGenerator::generateUsingRule(Symbol production, const Rule &rule) -> void
    {
        const auto &rule_first_set =
            firstSetEvaluator.getFirstSetOfRules().at(std::addressof(rule));

        auto set = rule_first_set;

        if (set.contains(storage.getEpsilon())) {
            set.insert_range(followSetEvaluator.getFollowSet().at(production));
        }

        for (auto symbol : set) {
            if (!storage.isTerminal(symbol) || symbol == storage.getEpsilon()) {
                continue;
            }

            insertIntoTable(
                {
                    .state = production,
                    .symbol = symbol,
                },
                std::addressof(rule));
        }
    }

    auto Ll1ParserGenerator::insertIntoTable(TableEntry entry, const Rule *rule) -> void
    {
        auto [it, inserted] = table.try_emplace(entry, rule);

        if (!inserted) {
            throw std::runtime_error{"Grammar is not LL(1) backtrack free."};
        }
    }
}// namespace ccl::parser::ll
