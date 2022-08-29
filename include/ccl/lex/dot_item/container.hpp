#ifndef CCL_PROJECT_CONTAINER_HPP
#define CCL_PROJECT_CONTAINER_HPP

#include <boost/container/small_vector.hpp>
#include <ccl/lex/analysis_shared.hpp>
#include <ccl/lex/dot_item/items_counter.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    class Container : public BasicItem
    {
    private:
        using BasicItem::analysis_shared;
        using BasicItem::canBeOptimized;
        using BasicItem::recurrence;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::TextIterator;

        using storage_t = boost::container::small_vector<std::unique_ptr<BasicItem>, 4>;

    public:
        Container(
            const TextIterator &rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = true)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(rule_iterator);
        }

        Container(
            TextIterator &&rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = true)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
        }

        Container(
            TextIterator &rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = false)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
        }

        [[nodiscard]] auto operator==(const Container &other) const noexcept
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const Container &other) const noexcept
        {
            return id <=> other.id;
        }

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return items.empty();
        }

        [[nodiscard]] auto getItems() const noexcept -> const storage_t &
        {
            return items;
        }

    private:
        template<typename T>
        requires std::is_base_of_v<BasicItem, T>
        auto unsafeGetLastItemAs() noexcept -> T *
        {
            // NOLINTNEXTLINE unsafe cast to increase performance
            return static_cast<T *>(items.back().get());
        }

        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool override;

        [[nodiscard]] static auto
            scanItem(const BasicItem *item, TextIterator &text_iterator, Token &token) -> bool;

        [[nodiscard]] static auto hasMovedToTheNextChar(TextIterator &rule_iterator) -> bool;

        auto parseRule(TextIterator &rule_iterator) -> void;

        auto recognizeAction(TextIterator &rule_iterator, ItemsCounter &items_counter) -> void;

        [[nodiscard]] auto
            constructNewSequence(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto
            constructNewUnion(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto
            constructNewItem(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        auto constructString(ItemsCounter &items_counter, bool is_character, bool is_multiline)
            -> void;

        auto constructTerminal(TextIterator &rule_iterator, ItemsCounter &items_counter) -> void;

        auto constructComment(ItemsCounter &items_counter) -> void;

        auto constructCommentOrCharacter(TextIterator &rule_iterator, ItemsCounter &items_counter)
            -> void;

        auto emplaceItem(std::unique_ptr<BasicItem> &&item) -> void;

        auto addPrefixPostfix() -> void;

        auto addRecurrence(TextIterator &rule_iterator, Recurrence new_recurrence) -> void;

        auto reverseLastItem(TextIterator &rule_iterator) -> void;

        auto postCreationCheck(TextIterator &rule_iterator, const ItemsCounter &counter) -> void;

        static auto findContainerEnd(TextIterator &rule_iterator, string_view repr) -> size_t;

        auto checkAbilityToCreatePrefixPostfix(TextIterator &rule_iterator) -> void;

        static auto throwUnableToApply(
            TextIterator &rule_iterator, string_view reason, string_view suggestion = {}) -> void;

        static auto throwUndefinedAction(TextIterator &rule_iterator) -> void;

        storage_t items{};
        size_t id{};
        bool main_item{};
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_HPP */
