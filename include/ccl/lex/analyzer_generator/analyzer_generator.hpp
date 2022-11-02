#ifndef CCL_PROJECT_ANALYZER_GENERATOR_HPP
#define CCL_PROJECT_ANALYZER_GENERATOR_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/string_view.hpp>
#include <filesystem>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(
        GenToken, size_t, EOI, BAD_TOKEN, IDENTIFIER, INTEGER, NEW_LINE, COLUMN, ASSIGN,
        CURLY_OPENING, CURLY_CLOSING, SEQUENCE, UNION, COMMENT, MULTILE_STRING, CHARACTER,
        RULE_DECLARATION, STRING);

    class AnalyzerGenerator
    {
        static LexicalAnalyzer LexForGenerator;// NOLINT non const variable

    public:
        static auto generateStaticVersion(const std::filesystem::path &path)
            -> Pair<std::string, std::string>;
        static auto generateStaticVersion(string_view filename, string_view text)
            -> Pair<std::string, std::string>;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
