#ifndef CCL_PROJECT_RULES_INFO_HPP
#define CCL_PROJECT_RULES_INFO_HPP

#include <atomic>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <isl/isl.hpp>

namespace ccl::parser::reader
{
    class RulesConstructor
    {
    private:
        isl::Map<std::size_t, std::string> ruleIdToName;
        isl::Map<isl::string_view, std::size_t> ruleNameToId;
        std::atomic<std::size_t> ruleIdGenerator;
        lexer::LexicalAnalyzer lexicalAnalyzer;
        GrammarRulesStorage grammarRulesStorage;

    public:
        auto addRule(isl::string_view rule_name) -> std::size_t
        {
            if (auto it = ruleNameToId.find(rule_name); it != ruleNameToId.end()) {
                return it->second;
            }

            const auto rule_id = ruleIdGenerator.fetch_add(1U, std::memory_order_relaxed);

            auto [it, has_inserter] = ruleIdToName.try_emplace(rule_id, rule_name);
            ruleNameToId.try_emplace(it->second, rule_id);

            return rule_id;
        }

        [[nodiscard]] auto getRuleName(std::size_t rule_id) const -> const std::string &
        {
            return ruleIdToName.at(rule_id);
        }

        [[nodiscard]] auto getRuleId(isl::string_view rule_name) const -> std::size_t
        {
            return ruleNameToId.at(rule_name);
        }

        auto addLexerRule(isl::string_view name, isl::UniquePtr<lexer::rule::Container> container)
            -> void
        {
            addRule(name);
            lexicalAnalyzer.addContainer(name, std::move(container));
        }

        auto addParserRule(isl::string_view name, parser::Rule rule) -> void
        {
            const auto rule_id = addRule(name);
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
