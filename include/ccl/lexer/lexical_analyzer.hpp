#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lexer/rule/container.hpp>
#include <utility>

namespace ccl::lexer
{
    template<typename T>
    CCL_DECL auto lexerEnumToString(SmallId value) -> std::string;

    template<>
    [[nodiscard]] inline auto lexerEnumToString<SmallId>(SmallId value) -> std::string
    {
        return fmt::to_string(value);
    }

    class LexicalAnalyzer
    {
    private:
        using Container = rule::Container;
        using RuleBlockInterface = rule::RuleBlockInterface;
        using TextIterator = RuleBlockInterface::TextIterator;

        struct Rule
        {
            std::string name;
            isl::string_view repr;
            SmallId id{};

            template<typename T>
            constexpr Rule(T rule_id, const isl::string_view rule_repr)
              : name{lexerEnumToString<T>(rule_id)}
              , repr{rule_repr}
              , id{static_cast<SmallId>(rule_id)}
            {}
        };

        ankerl::unordered_dense::map<std::string, Container *> allItemsMap;
        std::vector<std::unique_ptr<Container>> items;
        AnyPlaceItems anyPlaceItems;
        std::vector<SmallId> ignoredIds;
        mutable std::string skippedCharacters;

        // NOLINTNEXTLINE reference
        const ExceptionHandler *exceptionHandler;

    public:
        class Tokenizer;
        class PegParser;

        [[nodiscard]] explicit LexicalAnalyzer(const ExceptionHandler &exception_handler);

        [[nodiscard]] LexicalAnalyzer(
            const ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
            isl::string_view filename = {}, std::vector<SmallId> ignored_ids = {});

        [[nodiscard]] auto shareAnyPlaceItems() CCL_LIFETIMEBOUND -> AnyPlaceItems &
        {
            return anyPlaceItems;
        }

        auto addContainer(const std::string &rule_name, std::unique_ptr<Container> new_container)
            -> void;

        [[nodiscard]] auto getIgnoredIds() const CCL_LIFETIMEBOUND -> const std::vector<SmallId> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getByRuleName(const std::string &name) const CCL_LIFETIMEBOUND
            -> const Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto getByRuleName(const std::string &name) CCL_LIFETIMEBOUND -> Container *
        {
            return allItemsMap.at(name);
        }

        [[nodiscard]] auto hasRule(const std::string &name) const -> bool
        {
            return allItemsMap.contains(name);
        }

        [[nodiscard]] auto getTokenizer(isl::string_view text, isl::string_view filename = {}) const
            CCL_LIFETIMEBOUND -> Tokenizer;

        [[nodiscard]] auto getTokenizer(
            isl::string_view text, isl::string_view filename,
            ExceptionHandler &handler) const CCL_LIFETIMEBOUND -> Tokenizer;

        [[nodiscard]] auto getItems() const noexcept
            -> const std::vector<std::unique_ptr<Container>> &
        {
            return items;
        }

    private:
        auto createContainer(const Rule &rule, u32 id, isl::string_view filename) -> void;
    };

    using Tokenizer = LexicalAnalyzer::Tokenizer;
}// namespace ccl::lexer

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
