#include <ccl/parser/ll/gll.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
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

    auto ParserBuilder::buildLr1() -> LrParser
    {
        finishGrammar(Mode::LR);

        return LrParser{
            getStartItem(),
            grammarRulesStorage.getEpsilon(),
            grammarRulesStorage,
            getIdToNameTranslationFunction(),
        };
    }

    auto ParserBuilder::buildGlr() -> GlrParser
    {
        finishGrammar(Mode::LR);

        return GlrParser{
            getStartItem(),
            grammarRulesStorage.getEpsilon(),
            grammarRulesStorage,
            getIdToNameTranslationFunction(),
        };
    }

    auto ParserBuilder::buildLl1() -> Ll1Parser
    {
        finishGrammar(Mode::LL);

        return Ll1Parser{
            getRuleId("GOAL"),
            grammarRulesStorage,
            getIdToNameTranslationFunction(),
        };
    }

    auto ParserBuilder::buildGLL() -> GllParser
    {
        finishGrammar(Mode::LR);

        return GllParser{
            getRuleId("GOAL"),
            grammarRulesStorage,
            getIdToNameTranslationFunction(),
        };
    }

    auto ParserBuilder::finishGrammar(Mode mode) -> void
    {
        if (!rulesConstructorFinalized) {
            rulesConstructorMode = mode;
            rulesConstructorFinalized = true;
            grammarRulesStorage.finishGrammar(getRuleId("GOAL"), mode == Mode::LR);
        }

        if (mode != rulesConstructorMode) {
            throw std::runtime_error{"Constructor mode can not be changed after initialization"};
        }
    }

    auto ParserBuilder::addRule(const std::string &rule_name) -> SmallId
    {
        if (auto it = ruleNameToId.find(rule_name); it != ruleNameToId.end()) {
            return it->second;
        }

        const auto rule_id = ruleIdGenerator.next();

        ruleIdToName.try_emplace(rule_id, rule_name);
        ruleNameToId.try_emplace(rule_name, rule_id);

        return rule_id;
    }

    auto ParserBuilder::getStartItem() const -> LrItem
    {
        const auto goal_id = getRuleId("GOAL");
        const auto &start_rules = grammarRulesStorage.at(goal_id);

        if (start_rules.size() > 1) {
            throw std::runtime_error{"Grammar must define only one goal production."};
        }

        if (start_rules.empty()) {
            throw std::runtime_error{"No goal production found."};
        }

        auto first_rule_it = grammarRulesStorage.at(goal_id).begin();

        return LrItem{
            .dottedRule =
                {
                    .rule = std::addressof(*first_rule_it),
                    .dotPosition = 0,
                },
            .production = goal_id,
            .lookAhead = 0,
        };
    }

    auto
        ParserBuilder::getIdToNameTranslationFunction() const -> std::function<std::string(SmallId)>
    {
        return [this](SmallId rule_id) {
            return ruleIdToName.at(rule_id);
        };
    }
}// namespace ccl::parser::reader
