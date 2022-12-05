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
        IDENTIFIER,
        INTEGER,
        NEW_LINE,
        COLUMN,
        ASSIGN,
        CURLY_OPENING,
        CURLY_CLOSING,
        SEQUENCE,
        UNION,
        COMMENT,
        MULTILINE_STRING,
        CHARACTER,
        RULE_DECLARATION,
        STRING
    };

    class AnalyzerGenerator
    {
        static LexicalAnalyzer lexForGenerator;// NOLINT non const variable

    public:
        static auto generateStaticVersion(const std::filesystem::path &path)
            -> Pair<std::string, std::string>;
        static auto generateStaticVersion(string_view filename, string_view text)
            -> Pair<std::string, std::string>;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
