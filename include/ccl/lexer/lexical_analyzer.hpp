#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lexer/rule/container.hpp>

namespace ccl::lexer
{
    template<typename T>
    CCL_DECL auto lexerEnumToString(T value) -> isl::string_view;

    template<>
    [[nodiscard]] inline auto lexerEnumToString<std::size_t>(std::size_t value) -> isl::string_view
    {
        return isl::as<isl::string_view>(fmt::to_string(value));
    }

    class LexicalAnalyzer
    {
    private:
        using Container = rule::Container;
        using RuleBlockInterface = rule::RuleBlockInterface;
        using TextIterator = typename RuleBlockInterface::TextIterator;

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

        isl::Map<isl::string_view, Container *> allItemsMap;
        isl::Vector<isl::UniquePtr<Container>> items;
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
            isl::string_view filename = {}, isl::Vector<Id> ignored_ids = {});

        [[nodiscard]] auto shareAnyPlaceItems() CCL_LIFETIMEBOUND -> AnyPlaceItems &
        {
            return anyPlaceItems;
        }

        [[nodiscard]] auto getIgnoredIds() const CCL_LIFETIMEBOUND -> const isl::Vector<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto
            getByRuleName(isl::string_view name) const CCL_LIFETIMEBOUND -> const Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto getByRuleName(isl::string_view name) CCL_LIFETIMEBOUND -> Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto getTokenizer(isl::string_view text, isl::string_view filename = {})
            CCL_LIFETIMEBOUND -> Tokenizer;

        [[nodiscard]] auto getTokenizer(
            isl::string_view text, isl::string_view filename,
            ExceptionHandler &handler) CCL_LIFETIMEBOUND -> Tokenizer;

        [[nodiscard]] auto getParser(
            isl::string_view rule_name, isl::string_view text, isl::string_view filename,
            ExceptionHandler &handler) CCL_LIFETIMEBOUND -> PegParser;

        [[nodiscard]] auto getParser(
            isl::string_view rule_name, isl::string_view text,
            isl::string_view filename = {}) CCL_LIFETIMEBOUND -> PegParser;

        [[nodiscard]] auto
            getItems() const noexcept -> const isl::Vector<isl::UniquePtr<Container>> &
        {
            return items;
        }

        [[nodiscard]] auto generateIdForItem() -> std::size_t
        {
            // TODO: implement
            return 0;
        }

    private:
        auto createContainer(Rule rule, Id id, isl::string_view filename) -> void;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
