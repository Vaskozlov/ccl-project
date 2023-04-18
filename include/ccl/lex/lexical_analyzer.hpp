#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex
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

        Vector<Container> items{};
        SpecialItems specialItems{};
        FlatSet<Id> ignoredIds{};
        std::string skippedCharacters{};
        ExceptionHandler &exceptionHandler;// NOLINT

    public:
        class Tokenizer;

        LexicalAnalyzer(
            ExceptionHandler &exception_handler, InitializerList<Rule> rules,
            string_view filename = {}, FlatSet<Id> ignored_ids = {});

        [[nodiscard]] auto getIgnoredIds() const -> const FlatSet<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getTokenizer(string_view text, string_view filename = {}) -> Tokenizer;

        [[nodiscard]] auto
            getTokenizer(string_view text, string_view filename, ExceptionHandler &handler)
                -> Tokenizer;

    private:
        auto createContainer(string_view rule, Id id, string_view filename) -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
