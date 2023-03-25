#ifndef CCL_PROJECT_ANALYZER_GENERATOR_HPP
#define CCL_PROJECT_ANALYZER_GENERATOR_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/string_view.hpp>
#include <filesystem>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    enum GenToken : Id
    {
        EOI = 0,
        BAD_TOKEN = 1,
        RULE,
        BAD_RULE_DECLARATION,
        DIRECTIVE,
        BAD_DIRECTIVE_DECLARATION,
        BAD_RULE_OR_DIRECTIVE_DECLARATION,
        GROUP_DECLARATION,
        BAD_GROUP_DECLARATION_ONLY_BRACKET,
        BAD_GROUP_DECLARATION_BRACKET_AND_NAME,
        BAD_GROUP_DECLARATION_EMPTY_NAME,
        BAD_GROUP_NO_OPEN_BRACKET,
    };

    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path) -> std::string;
        static auto generateStaticVersion(string_view filename, string_view text) -> std::string;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
