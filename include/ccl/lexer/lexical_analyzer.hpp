#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lexer/dot_item/container.hpp>

namespace ccl::lexer
{
    class LexicalAnalyzer
    {
    private:
        using Container = dot_item::Container;
        using DotItemConcept = dot_item::DotItemConcept;
        using TextIterator = typename DotItemConcept::TextIterator;

        struct CCL_TRIVIAL_ABI Rule
        {
            Id id{};
            string_view repr{};
        };

        std::vector<Container> items{};
        SpecialItems specialItems{};
        std::vector<Id> ignoredIds{};
        std::string skippedCharacters{};
        ExceptionHandler &exceptionHandler;// NOLINT

    public:
        class Tokenizer;

        [[nodiscard]] LexicalAnalyzer(
            ExceptionHandler &exception_handler, InitializerList<Rule> rules,
            std::string_view filename = {}, std::vector<Id> ignored_ids = {});

        [[nodiscard]] auto getIgnoredIds() const -> const std::vector<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getTokenizer(string_view text, std::string_view filename = {})
            -> Tokenizer;

        [[nodiscard]] auto
            getTokenizer(string_view text, std::string_view filename, ExceptionHandler &handler)
                -> Tokenizer;

    private:
        auto createContainer(string_view rule, Id id, std::string_view filename) -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
