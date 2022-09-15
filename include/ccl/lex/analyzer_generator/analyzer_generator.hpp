#ifndef CCL_PROJECT_ANALYZER_GENERATOR_HPP
#define CCL_PROJECT_ANALYZER_GENERATOR_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/string_view.hpp>
#include <filesystem>
#include <string>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(
        GenToken, size_t, EOI, BAD_TOKEN, IDENTIFIER, INTEGER, NEW_LINE, COLUMN, ASSIGN,
        CURLY_OPENING, CURLY_CLOSING, SEQUENCE, UNION, COMMENT, MULTILE_STRING, CHARACTER,
        RULE_DECLARATION, STRING);

    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path)
            -> std::pair<std::string, std::string>;
        static auto generateStaticVersion(string_view filename, string_view text)
            -> std::pair<std::string, std::string>;

    private:
        static LexicalAnalyzer LexForGenerator;// NOLINT non const variable
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
