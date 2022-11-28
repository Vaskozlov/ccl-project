#ifndef CCL_PROJECT_STATIC_GENERATOR_HPP
#define CCL_PROJECT_STATIC_GENERATOR_HPP

#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <map>
#include <stack>

namespace ccl::lex::gen
{
    class StaticGenerator
    {
    public:
        using Tokenizer = typename LexicalAnalyzer::Tokenizer;

    private:
        Tokenizer &tokenizer;
        parser::CcllParser ccllParser;
        std::string includeDirForSrc{};
        std::string handler{"ccl::handler::Cmd::instance()"};
        std::string variableName{"set_me"};
        std::string filename{"set_me"};
        std::string nameSpace{};
        std::string enumName{};
        std::string generatedHeader{};
        std::string generatedSource{};
        std::string extraSpaces{};

    public:
        explicit StaticGenerator(Tokenizer &input_tokenizer)
          : tokenizer{input_tokenizer}
          , ccllParser{tokenizer}
        {
            generate();
        }

        [[nodiscard]] auto get() const noexcept -> Pair<std::string, std::string>
        {
            return {generatedHeader, generatedSource};
        }

    private:
        auto loadDirectives() -> void;

        auto generate() -> void;
        auto generateHeaderDefinition() -> void;

        auto generateNamespaceBegin(std::string &string) -> void;
        auto generateNamespaceEnd(std::string &string) -> void;

        auto generateRuleNames(std::string &string, std::string addition_flags = {}) -> void;

        auto generateEnum() -> void;
        auto generateEnumCases() -> void;

        auto generateVariable() -> void;
        auto generateLexicalAnalyzer(std::string &string, std::string addition_flags = {}) -> void;

        auto generateRules(std::string &string) -> void;
        auto generateSource() -> void;

        auto appendToStr(std::string &dest, const std::string &string) const -> void;
    };
}// namespace ccl::lex::gen

#endif /* CCL_PROJECT_STATIC_GENERATOR_HPP */
