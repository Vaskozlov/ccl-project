#ifndef CCL_PROJECT_RULES_READER_HPP
#define CCL_PROJECT_RULES_READER_HPP

#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/reader_options.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>

namespace ccl::parser::reader
{
    class RulesReader
    {
    private:
        RulesConstructor rulesConstructor;
        std::vector<ReaderOption> readerOptions;

    public:
        explicit RulesReader(isl::string_view input, isl::string_view filename = {});

        [[nodiscard]] auto getRulesConstructor() -> RulesConstructor &
        {
            return rulesConstructor;
        }
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_RULES_READER_HPP */
