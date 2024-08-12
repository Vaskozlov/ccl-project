#ifndef CCL_PROJECT_RULES_INFO_HPP
#define CCL_PROJECT_RULES_INFO_HPP

#include <atomic>
#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/detail/lr_item.hpp>
#include <isl/isl.hpp>
#include <isl/thread/id_generator.hpp>

namespace ccl::parser::reader
{
    class RulesConstructor
    {
    private:
        lexer::LexicalAnalyzer lexicalAnalyzer{handler::Cmd::instance()};
        std::map<SmallId, std::string> ruleIdToName;
        std::map<isl::string_view, SmallId> ruleNameToId;
        isl::thread::IdGenerator<SmallId> ruleIdGenerator{lexer::ReservedTokenMaxValue + 1};
        GrammarRulesStorage grammarRulesStorage{addRule("EPSILON")};

    public:
        auto addRule(isl::string_view rule_name) -> SmallId
        {
            if (auto it = ruleNameToId.find(rule_name); it != ruleNameToId.end()) {
                return it->second;
            }

            const auto rule_id = ruleIdGenerator.next();

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
            return LrItem{std::addressof(grammarRulesStorage.at(goal_id).front()), 0, goal_id, 0};
        }

        auto getIdToNameTranslationFunction() const CCL_LIFETIMEBOUND
            -> std::function<std::string(SmallId)>
        {
            return [this](SmallId rule_id) {
                return ruleIdToName.at(rule_id);
            };
        }

        [[nodiscard]] auto getRuleName(SmallId rule_id) const -> const std::string &
        {
            return ruleIdToName.at(rule_id);
        }

        [[nodiscard]] auto getRuleId(isl::string_view rule_name) const -> SmallId
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
            addParserRule(rule_id, std::move(rule));
        }

        auto addParserRule(SmallId rule_id, parser::Rule rule) -> void
        {
            grammarRulesStorage.tryEmplace(rule_id, std::move(rule));
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
