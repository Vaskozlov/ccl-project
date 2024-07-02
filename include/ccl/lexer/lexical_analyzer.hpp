#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lexer/dot_item/container.hpp>

namespace ccl::lexer
{
    template<typename T>
    CCL_DECL auto lexerEnumToString(T value) -> isl::string_view;

    template<>
    CCL_DECL auto lexerEnumToString<std::size_t>(std::size_t value) -> isl::string_view
    {
        return isl::as<isl::string_view>(std::to_string(value));
    }

    class LexicalAnalyzer
    {
    private:
        using Container = dot_item::Container;
        using DotItemConcept = dot_item::DotItemConcept;
        using TextIterator = typename DotItemConcept::TextIterator;

        struct CCL_TRIVIAL_ABI Rule
        {
            Id id{};
            isl::string_view repr;
            isl::string_view name;

            template<typename T>
            constexpr Rule(T rule_id, isl::string_view rule_repr)
              : id{isl::as<Id>(rule_id)}
              , repr{rule_repr}
              , name{lexerEnumToString<T>(rule_id)}
            {}
        };

        std::map<isl::string_view, Container *> allItemsMap;
        isl::Vector<std::unique_ptr<Container>> items;
        AnyPlaceItems anyPlaceItems;
        isl::Vector<Id> ignoredIds;
        std::string skippedCharacters;

        // NOLINTNEXTLINE reference
        ExceptionHandler &exceptionHandler;

    public:
        class Tokenizer;
        class PegParser;

        [[nodiscard]] LexicalAnalyzer(
            ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
            std::string_view filename = {}, isl::Vector<Id> ignored_ids = {});

        [[nodiscard]] auto getIgnoredIds() const -> const isl::Vector<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getByRuleName(isl::string_view name) const -> const Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto getByRuleName(isl::string_view name) -> Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto
            getTokenizer(isl::string_view text, std::string_view filename = {}) -> Tokenizer;

        [[nodiscard]] auto getTokenizer(
            isl::string_view text, std::string_view filename,
            ExceptionHandler &handler) -> Tokenizer;

        [[nodiscard]] auto getParser(
            isl::string_view rule_name, isl::string_view text, std::string_view filename,
            ExceptionHandler &handler) -> PegParser;

        [[nodiscard]] auto getParser(
            isl::string_view rule_name, isl::string_view text,
            std::string_view filename = {}) -> PegParser;

    private:
        auto createContainer(Rule rule, Id id, std::string_view filename) -> void;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
