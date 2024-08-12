#include <ccl/parser/rules_reader/parser_builder.hpp>

namespace ccl::parser::reader
{
    ParserBuilder::ParserBuilder()
      : ruleIdToName{
            {
                std::to_underlying(lexer::ReservedTokenType::EOI),
                "EOI",
            },
            {
                std::to_underlying(lexer::ReservedTokenType::BAD_TOKEN),
                "BAD_TOKEN",
            },
            {
                std::to_underlying(lexer::ReservedTokenType::CUT),
                "CUT",
            },
        },
      ruleNameToId{
          {
              "EOI",
              std::to_underlying(lexer::ReservedTokenType::EOI),
          },
          {
              "BAD_TOKEN",
              std::to_underlying(lexer::ReservedTokenType::BAD_TOKEN),
          },
          {
              "CUT",
              std::to_underlying(lexer::ReservedTokenType::CUT),
          },
      },
      ruleIdGenerator{lexer::ReservedTokenMaxValue + 1},
      grammarRulesStorage{addRule("EPSILON")}
    {}

    auto ParserBuilder::finishGrammar(Mode mode) -> void
    {
        grammarRulesStorage.finishGrammar(mode == Mode::LR);
    }

    auto ParserBuilder::addRule(isl::string_view rule_name) -> SmallId
    {
        if (auto it = ruleNameToId.find(rule_name); it != ruleNameToId.end()) {
            return it->second;
        }

        const auto rule_id = ruleIdGenerator.next();

        auto [it, has_inserter] = ruleIdToName.try_emplace(rule_id, rule_name);
        ruleNameToId.try_emplace(it->second, rule_id);

        return rule_id;
    }

    auto ParserBuilder::getStartItem() const -> LrItem
    {
        const auto goal_id = getRuleId("GOAL");
        auto &start_rules = grammarRulesStorage.at(goal_id);

        if (start_rules.size() > 1) {
            throw std::runtime_error{"Grammar must define only one goal production."};
        }

        if (start_rules.empty()) {
            throw std::runtime_error{"No goal production found."};
        }

        return LrItem{std::addressof(grammarRulesStorage.at(goal_id).front()), 0, goal_id, 0};
    }

    auto
        ParserBuilder::getIdToNameTranslationFunction() const -> std::function<std::string(SmallId)>
    {
        return [this](SmallId rule_id) {
            return ruleIdToName.at(rule_id);
        };
    }
}// namespace ccl::parser::reader
