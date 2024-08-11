#ifndef CCL_PROJECT_PEG_PARSER_HPP
#define CCL_PROJECT_PEG_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node.hpp>

namespace ccl::lexer
{
    class LexicalAnalyzer::PegParser
    {
    private:
        TextIterator textIterator;
        const Container *mainItem{};

    public:
        [[nodiscard]] PegParser(
            isl::string_view rule_name, const LexicalAnalyzer &lexical_analyzer,
            isl::string_view text, isl::string_view filename = {});

        [[nodiscard]] PegParser(
            isl::string_view rule_name, const LexicalAnalyzer &lexical_analyzer,
            isl::string_view text, isl::string_view filename, ExceptionHandler &exception_handler);

        [[nodiscard]] auto parse() -> isl::UniquePtr<parser::ast::Node>;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_PEG_PARSER_HPP */
