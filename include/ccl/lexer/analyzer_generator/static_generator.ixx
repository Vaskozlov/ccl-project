module;
#include <ccl/defines.hpp>
export module ccl.lexer.generator:static_generator;

export import ccl.lexer;
export import ccl.codegen;
export import :analyzer_generator;
export import :ccll_parser;

export namespace ccl::lexer::gen
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
}// namespace ccl::lexer::gen
