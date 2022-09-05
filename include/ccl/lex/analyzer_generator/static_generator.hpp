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

        explicit StaticGenerator(Tokenizer &tokenizer_)
          : tokenizer(tokenizer_), ccll_parser(tokenizer)
        {
            generateHeader();
        }

    private:
        auto loadDirectives() -> void;

        auto generateHeader() -> void;
        auto generateHeaderDefinition() -> void;

        auto generateEnum() -> void;
        auto generateEnumCases() -> void;

        auto generateVariable() -> void;
        auto generateRules() -> void;

        string_view handler{ "ccl::handler::Cmd::instance()" };
        string_view filename{ "set_me" };
        string_view variable_name{ "set_me" };
        std::string enum_name{};
        Tokenizer &tokenizer;
        parser::CcllParser ccll_parser;
        std::string generated_header{};
    };
}// namespace ccl::lex::gen

#endif /* CCL_PROJECT_STATIC_GENERATOR_HPP */
