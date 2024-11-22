#ifndef CCL_PROJECT_RULES_READER_HPP
#define CCL_PROJECT_RULES_READER_HPP

#include <ccl/parser/rules_reader/parser_builder.hpp>
#include <ccl/parser/rules_reader/reader_options.hpp>

namespace ccl::parser::reader
{
    auto getRulesGrammar() -> const GrammarStorage &;

    class RulesReader
    {
    private:
        ParserBuilder rulesConstructor;
        std::vector<ReaderOption> readerOptions;

    public:
        explicit RulesReader(
            const std::unordered_map<std::string, SmallId> &default_rules,
            isl::string_view input, isl::string_view filename = {});

        [[nodiscard]] auto getParserBuilder() -> ParserBuilder &;
    };
}// namespace ccl::parser::reader

#endif /* CCL_PROJECT_RULES_READER_HPP */
