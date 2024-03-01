module;
#include <ccl/defines.hpp>
export module ccl.lexer:lexical_analyzer;

export import isl;
export import ccl.handler;
export import ccl.lexer.dot_item;

export namespace ccl::lexer
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
            isl::string_view repr{};
        };

        std::vector<Container> items{};
        dot_item::SpecialItems specialItems{};
        std::vector<Id> ignoredIds{};
        std::string skippedCharacters{};
        handler::ExceptionHandler &exceptionHandler;// NOLINT

    public:
        class Tokenizer;

        [[nodiscard]] LexicalAnalyzer(
            handler::ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
            std::string_view filename = {}, std::vector<Id> ignored_ids = {});

        [[nodiscard]] auto getIgnoredIds() const -> const std::vector<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto
            getTokenizer(isl::string_view text, std::string_view filename = {}) -> Tokenizer;

        [[nodiscard]] auto getTokenizer(
            isl::string_view text, std::string_view filename,
            handler::ExceptionHandler &handler) -> Tokenizer;

    private:
        auto createContainer(isl::string_view rule, Id id, std::string_view filename) -> void;
    };
}// namespace ccl::lexer
