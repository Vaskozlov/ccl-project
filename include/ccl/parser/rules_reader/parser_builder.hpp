#ifndef CCL_PROJECT_PARSER_BUILDER_HPP
#define CCL_PROJECT_PARSER_BUILDER_HPP

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/grammar_slot.hpp>
#include <isl/thread/id_generator.hpp>

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
        std::map<SmallId, std::string> ruleIdToName;
        std::map<isl::string_view, SmallId> ruleNameToId;
        isl::thread::IdGenerator<SmallId> ruleIdGenerator;
        GrammarStorage grammarRulesStorage;
        Mode rulesConstructorMode{Mode::LR};
        bool rulesConstructorFinalized{};

    public:
        ParserBuilder();

        auto buildLr1() -> LrParser;

        auto buildGlr() -> GlrParser;

        auto buildLl1() -> Ll1Parser;

        auto buildGLL() -> void;

        auto addRule(isl::string_view rule_name) -> SmallId;

        auto getIdToNameTranslationFunction() const CCL_LIFETIMEBOUND
            -> std::function<std::string(SmallId)>;

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

        [[nodiscard]] auto getGrammarRulesStorage() const noexcept -> const GrammarStorage &
        {
            return grammarRulesStorage;
        }

    private:
        auto finishGrammar(Mode mode) -> void;

        auto getStartItem() const -> GrammarSlot;
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_PARSER_BUILDER_HPP */
