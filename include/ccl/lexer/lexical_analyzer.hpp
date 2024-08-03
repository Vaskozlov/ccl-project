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
            isl::string_view repr;
            isl::string_view name;
            u32 id{};

            template<typename T>
            constexpr Rule(T rule_id, isl::string_view rule_repr)
              : repr{rule_repr}
              , name{lexerEnumToString<T>(rule_id)}
              , id{isl::as<u32>(rule_id)}
            {}
        };

        std::map<isl::string_view, Container *> allItemsMap;
        std::vector<isl::UniquePtr<Container>> items;
        AnyPlaceItems anyPlaceItems;
        std::vector<u32> ignoredIds;
        std::string skippedCharacters;
        std::atomic<u32> idCounter{0};

        // NOLINTNEXTLINE reference
        ExceptionHandler &exceptionHandler;

    public:
        class Tokenizer;
        class PegParser;

        [[nodiscard]] explicit LexicalAnalyzer(ExceptionHandler &exception_handler);

        [[nodiscard]] LexicalAnalyzer(
            ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
            isl::string_view filename = {}, std::vector<u32> ignored_ids = {});

        [[nodiscard]] auto shareAnyPlaceItems() CCL_LIFETIMEBOUND -> AnyPlaceItems &
        {
            return anyPlaceItems;
        }

        auto addContainer(isl::string_view rule_name, isl::UniquePtr<Container> new_container)
            -> void
        {
            allItemsMap.try_emplace(rule_name, new_container.get());

            if (new_container->isAnyPlaceItem()) {
                anyPlaceItems.items.emplace_back(std::move(new_container));
            } else {
                items.emplace_back(std::move(new_container));
            }
        }

        [[nodiscard]] auto getIgnoredIds() const CCL_LIFETIMEBOUND -> const std::vector<u32> &
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
            getItems() const noexcept -> const std::vector<isl::UniquePtr<Container>> &
        {
            return items;
        }

        [[nodiscard]] auto generateIdForItem() noexcept -> SmallId
        {
            return static_cast<SmallId>(idCounter.fetch_add(1U, std::memory_order_relaxed));
        }

    private:
        auto createContainer(const Rule &rule, u32 id, isl::string_view filename) -> void;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
