#ifndef CCL_PROJECT_RULES_INFO_HPP
#define CCL_PROJECT_RULES_INFO_HPP

#include <atomic>
#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/detail/lr_item.hpp>
#include <isl/isl.hpp>

namespace ccl::parser::reader
{
    class RulesConstructor
    {
    private:
        lexer::LexicalAnalyzer lexicalAnalyzer{handler::Cmd::instance()};
        isl::Map<Id, std::string> ruleIdToName;
        isl::Map<isl::string_view, Id> ruleNameToId;
        std::atomic<Id> ruleIdGenerator{lexer::ReservedTokenMaxValue + 1};
        GrammarRulesStorage grammarRulesStorage{addRule("EPSILON")};

    public:
        auto addRule(isl::string_view rule_name) -> Id
        {
            if (auto it = ruleNameToId.find(rule_name); it != ruleNameToId.end()) {
                return it->second;
            }

            const auto rule_id = ruleIdGenerator.fetch_add(1U, std::memory_order_relaxed);

            auto [it, has_inserter] = ruleIdToName.try_emplace(rule_id, rule_name);
            ruleNameToId.try_emplace(it->second, rule_id);

            return rule_id;
        }

        auto finishGrammar() -> void
        {
            grammarRulesStorage.finishGrammar();
        }

        auto getStartItem() const -> LrItem
        {
            const auto goal_id = getRuleId("GOAL");
            return LrItem{grammarRulesStorage.at(goal_id).front(), 0, goal_id, 0};
        }

        auto getIdToNameTranslationFunction() const CCL_LIFETIMEBOUND
            -> std::function<std::string(Id)>
        {
            return [this](Id rule_id) {
                return ruleIdToName.at(rule_id);
            };
        }

        [[nodiscard]] auto getRuleName(Id rule_id) const -> const std::string &
        {
            return ruleIdToName.at(rule_id);
        }

        [[nodiscard]] auto getRuleId(isl::string_view rule_name) const -> Id
        {
            return ruleNameToId.at(rule_name);
        }

        [[nodiscard]] auto hasRule(isl::string_view rule_name) const -> bool
        {
            return ruleNameToId.contains(rule_name);
        }

        auto addLexerRule(isl::string_view name, isl::UniquePtr<lexer::rule::Container> container)
            -> void
        {
            auto rule_id = addRule(name);
            container->setId(rule_id);
            lexicalAnalyzer.addContainer(name, std::move(container));
        }

        auto addParserRule(isl::string_view name, parser::Rule rule) -> void
        {
            const auto rule_id = addRule(name);
            grammarRulesStorage.try_emplace(rule_id);
            grammarRulesStorage.at(rule_id).emplace_back(std::move(rule));
        }

        auto addParserRule(Id rule_id, parser::Rule rule) -> void
        {
            grammarRulesStorage.try_emplace(rule_id);
            grammarRulesStorage.at(rule_id).emplace_back(std::move(rule));
        }

        [[nodiscard]] auto getLexicalAnalyzer() noexcept -> lexer::LexicalAnalyzer &
        {
            return lexicalAnalyzer;
        }

        [[nodiscard]] auto getLexicalAnalyzer() const noexcept -> const lexer::LexicalAnalyzer &
        {
            return lexicalAnalyzer;
        }

        [[nodiscard]] auto getGrammarRulesStorage() noexcept -> GrammarRulesStorage &
        {
            return grammarRulesStorage;
        }

        [[nodiscard]] auto getGrammarRulesStorage() const noexcept -> const GrammarRulesStorage &
        {
            return grammarRulesStorage;
        }
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_RULES_INFO_HPP */
