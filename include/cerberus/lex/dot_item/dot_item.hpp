#ifndef CERBERUS_PROJECT_DOT_ITEM_HPP
#define CERBERUS_PROJECT_DOT_ITEM_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/items_counter.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>
#include <cerberus/lex/dot_item/union.hpp>

namespace cerb::lex::dot_item
{
    class DotItem : public BasicItem
    {
    private:
        using BasicItem::analysis_shared;
        using BasicItem::canBeOptimized;
        using BasicItem::isNextCharNotForScanning;
        using BasicItem::recurrence;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::ScanStatus;
        using typename BasicItem::TextIterator;

        using storage_t = boost::container::small_vector<std::unique_ptr<BasicItem>, 4>;

    public:
        explicit DotItem(
            const TextIterator &rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = true)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(rule_iterator);
        }

        explicit DotItem(
            TextIterator &&rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = true)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
        }

        explicit DotItem(
            TextIterator &rule_iterator_, size_t id_, AnalysisShared &analysis_shared_,
            bool main_item_ = false)
          : BasicItem(analysis_shared_), id(id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
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
        auto unsafeGetLastItemAs() -> T *
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

        auto recognizeAction(TextIterator &rule_iterator, ItemsCounter &counter) -> void;

        [[nodiscard]] auto
            constructNewSequence(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto
            constructNewUnion(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto
            constructNewItem(TextIterator &rule_iterator, ItemsCounter &items_counter)
                -> std::unique_ptr<BasicItem>;

        auto emplaceItem(std::unique_ptr<BasicItem> &&item) -> void;

        auto addPrefixPostfix() -> void;

        auto constructString(TextIterator &rule_iterator, bool is_character, bool is_multiline)
            -> void;

        auto constructTerminal(TextIterator &rule_iterator) -> void;

        auto constructComment(TextIterator &rule_iterator) -> void;

        auto addRecurrence(TextIterator &rule_iterator, Recurrence new_recurrence) -> void;

        auto reverseLastItem(TextIterator &rule_iterator) -> void;

        auto postCheck(TextIterator &rule_iterator, const ItemsCounter &counter) -> void;

        static auto findDotItemEnd(TextIterator &rule_iterator, u8string_view repr) -> size_t;

        auto checkThereIsOnlySequence(TextIterator &rule_iterator, u8string_view modifier) -> void;

        auto checkAbilityToCreatePrefixPostfix(TextIterator &rule_iterator) -> void;

        auto checkNotEmpty(
            TextIterator &rule_iterator, u8string_view message, u8string_view suggestion = {})
            -> void;

        static auto throwUnableToApply(
            TextIterator &rule_iterator,
            u8string_view reason,
            u8string_view suggestion = {}) -> void;

        template<ConstString Reason, ConstString Suggestion = u8"">
        static auto throwUnableToApply(TextIterator &rule_iterator) -> void;

        static auto throwUndefinedAction(TextIterator &rule_iterator) -> void;

        storage_t items{};
        size_t id{};
        bool main_item{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
