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
            generate();
        }

        [[nodiscard]] auto get() const noexcept -> std::pair<std::string, std::string>
        {
            return { generated_header, generated_source };
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

        Tokenizer &tokenizer;
        parser::CcllParser ccll_parser;
        std::string include_dir_for_src{};
        std::string handler{ "ccl::handler::Cmd::instance()" };
        std::string variable_name{ "set_me" };
        std::string filename{ "set_me" };
        std::string name_space{};
        std::string enum_name{};
        std::string generated_header{};
        std::string generated_source{};
        std::string extra_spaces{};
    };
}// namespace ccl::lex::gen

#endif /* CCL_PROJECT_STATIC_GENERATOR_HPP */
