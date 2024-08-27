#ifndef CCL_PROJECT_PARSER_BUILDER_HPP
#define CCL_PROJECT_PARSER_BUILDER_HPP

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/lr_item.hpp>
#include <isl/id_generator.hpp>

namespace ccl::parser
{
    class LrParser;
    class GlrParser;
    class Ll1Parser;
    class GllParser;
}// namespace ccl::parser

namespace ccl::parser::reader
{
    enum class Mode : u8
    {
        LL,
        LR
    };

    class ParserBuilder
    {
    private:
        lexer::LexicalAnalyzer lexicalAnalyzer{handler::Cmd::instance()};
        ankerl::unordered_dense::map<SmallId, std::string> ruleIdToName;
        ankerl::unordered_dense::map<std::string, SmallId> ruleNameToId;
        isl::IdGenerator<SmallId> ruleIdGenerator;
        GrammarStorage grammarRulesStorage;
        Mode rulesConstructorMode{Mode::LR};
        bool rulesConstructorFinalized{};

    public:
        ParserBuilder();

        auto buildLr1() -> LrParser;

        auto buildGlr() -> GlrParser;

        auto buildLl1() -> Ll1Parser;

        auto buildGLL() -> GllParser;

        auto addRule(const std::string &rule_name) -> SmallId;

        auto getIdToNameTranslationFunction() const CCL_LIFETIMEBOUND
            -> std::function<std::string(SmallId)>;

        [[nodiscard]] auto getRuleName(SmallId rule_id) const -> const std::string &
        {
            return ruleIdToName.at(rule_id);
        }

        [[nodiscard]] auto getRuleId(const std::string &rule_name) const -> SmallId
        {
            return ruleNameToId.at(rule_name);
        }

        [[nodiscard]] auto hasRule(const std::string &rule_name) const -> bool
        {
            return ruleNameToId.contains(rule_name);
        }

        auto addLexerRule(
            const std::string &name, std::unique_ptr<lexer::rule::Container> container) -> void
        {
            auto rule_id = addRule(name);
            container->setId(rule_id);
            lexicalAnalyzer.addContainer(name, std::move(container));
        }

        auto addParserRule(const std::string &name, Rule rule) -> void
        {
            const auto rule_id = addRule(name);
            addParserRule(rule_id, std::move(rule));
        }

        auto addParserRule(SmallId rule_id, Rule rule) -> void
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

        [[nodiscard]] auto getGrammarRulesStorage() const noexcept -> const GrammarStorage &
        {
            return grammarRulesStorage;
        }

    private:
        auto finishGrammar(Mode mode) -> void;

        auto getStartItem() const -> LrItem;
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_PARSER_BUILDER_HPP */
