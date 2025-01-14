#include <ccl/parser/ll/ll_1_parser_generator.hpp>

namespace ccl::parser::ll
{
    Ll1ParserGenerator::Ll1ParserGenerator(
        const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
    {
        for (auto &[key, rule] : storage.rulesIterator()) {
            generateUsingRule(key, rule);
        }
    }

    auto Ll1ParserGenerator::generateUsingRule(const Symbol production, const Rule &rule) -> void
    {
        auto set = rule.getFirstSet();

        if (std::ranges::find(set, storage.getEpsilon()) != set.end()) {
            for (auto symbol : rule.getFollowSet()) {
                if (std::ranges::find(set, symbol) == set.end()) {
                    set.emplace_back(symbol);
                }
            }
        }

        for (const auto symbol : set) {
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

    auto Ll1ParserGenerator::insertIntoTable(const TableEntry entry, const Rule *rule) -> void
    {
        auto inserted = false;
        std::tie(std::ignore, inserted) = table.try_emplace(entry, rule);

        if (!inserted) {
            throw std::runtime_error{"Grammar is not LL(1) backtrack free."};
        }
    }
} // namespace ccl::parser::ll
