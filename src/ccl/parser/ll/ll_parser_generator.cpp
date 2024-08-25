#include <ccl/parser/ll/ll_parser_generator.hpp>

namespace ccl::parser::ll
{
    LlParserGenerator::LlParserGenerator(
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

    [[nodiscard]] auto LlParserGenerator::createLl1Table() const -> Ll1Table
    {
        auto ll_1_table = Ll1Table{};

        for (const auto &[entry, rules] : table) {
            if (rules.size() > 1) {
                throw std::runtime_error{"Grammar is not LL(1) backtrack free."};
            }

            ll_1_table.emplace(entry, rules.front());
        }

        return ll_1_table;
    }

    [[nodiscard]] auto LlParserGenerator::createGllTable() const -> GllTable
    {
        return GllTable{table.begin(), table.end()};
    }

    auto LlParserGenerator::generateUsingRule(Symbol production, const Rule &rule) -> void
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

    auto LlParserGenerator::insertIntoTable(TableEntry entry, const Rule *rule) -> void
    {
        auto [it, inserted] = table.try_emplace(entry);

        if (auto &vec = it->second; std::ranges::find(vec, rule) == vec.end()) {
            vec.emplace_back(rule);
        }
    }
}// namespace ccl::parser::ll
