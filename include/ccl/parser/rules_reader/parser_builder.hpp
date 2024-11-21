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
        GrammarStorage grammarRulesStorage;
        isl::IdGenerator<SmallId> ruleIdGenerator;
        Mode rulesConstructorMode{Mode::LR};
        bool rulesConstructorFinalized{};

    public:
        ParserBuilder(const std::map<std::string, SmallId> &default_rules);

        auto buildLr1() -> LrParser;

        auto buildGlr() -> GlrParser;

        auto buildLl1() -> Ll1Parser;

        auto buildGLL() -> GllParser;

        auto addRule(const std::string &rule_name) -> SmallId;

        [[nodiscard]] auto getIdToNameTranslationFunction() const CCL_LIFETIMEBOUND
            -> std::function<std::string(SmallId)>;

        [[nodiscard]] auto getStartRule() const CCL_LIFETIMEBOUND -> const Rule *;

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

        auto
            addLexerRule(const std::string &name, std::unique_ptr<lexer::rule::Container> container)
                -> void;

        auto addParserRule(const std::string &name, std::vector<Symbol> rule) -> void;

        auto addParserRule(const SmallId rule_id, std::vector<Symbol> rule) -> void
        {
            grammarRulesStorage.tryEmplace(rule_id, Rule{std::move(rule)});
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

        [[nodiscard]] auto getStartItem() const -> LrItem;
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_PARSER_BUILDER_HPP */
