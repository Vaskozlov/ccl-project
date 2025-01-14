#include <ccl/parser/ll/gll_parser_generator.hpp>

namespace ccl::parser::ll
{
    GllParserGenerator::GllParserGenerator(
        const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
    {
        for (auto &[key, rule] : storage.rulesIterator()) {
            generateUsingRule(key, rule);
        }
    }

    auto GllParserGenerator::generateUsingRule(const Symbol production, const Rule &rule) -> void
    {
        for (const auto symbol : rule.getFirstSet()) {
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

    auto GllParserGenerator::insertIntoTable(const TableEntry entry, const Rule *rule) -> void
    {
        auto [it, inserted] = table.try_emplace(entry);

        if (auto &vec = it->second; std::ranges::find(vec, rule) == vec.end()) {
            vec.emplace_back(rule);
        }
    }
} // namespace ccl::parser::ll
