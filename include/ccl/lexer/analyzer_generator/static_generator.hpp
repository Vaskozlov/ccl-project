#ifndef CCL_PROJECT_STATIC_GENERATOR_HPP
#define CCL_PROJECT_STATIC_GENERATOR_HPP

#include <ccl/codegen/basic_codegen.hpp>
#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/analyzer_generator/ccll_parser.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <map>
#include <stack>

namespace ccl::lexer::gen
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
        std::string extraSpaces{};
        codegen::BasicCodeGenerator codeGenerator{};

    public:
        explicit StaticGenerator(Tokenizer &input_tokenizer);

        [[nodiscard]] auto getCode() const noexcept -> std::string
        {
            return codeGenerator.getCode();
        }

    private:
        auto loadDirectives() -> void;

        auto generate() -> void;
        auto generateHeaderDefinition() -> void;

        auto generateNamespaceBegin() -> void;
        auto generateNamespaceEnd() -> void;

        auto generateRuleNames() -> void;

        auto generateEnum() -> void;
        auto generateEnumCases() -> void;

        auto generateVariable() -> void;
        auto generateLexicalAnalyzer() -> void;

        auto generateRules() -> void;
    };
}// namespace ccl::lex::gen

#endif /* CCL_PROJECT_STATIC_GENERATOR_HPP */
