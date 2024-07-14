#ifndef CCL_PROJECT_ANALYZER_GENERATOR_HPP
#define CCL_PROJECT_ANALYZER_GENERATOR_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <filesystem>
#include <isl/string_view.hpp>

namespace ccl::lexer
{
    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path) -> std::string;
        static auto
            generateStaticVersion(std::string_view filename, isl::string_view text) -> std::string;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
