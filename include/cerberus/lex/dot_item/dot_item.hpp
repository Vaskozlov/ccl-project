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
        using BasicItem::repetition;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::ScanStatus;
        using typename BasicItem::TextIterator;

    public:
        explicit DotItem(
            const TextIterator &rule_iterator_,
            size_t id_,
            AnalysisShared &analysis_shared_)
          : BasicItem(analysis_shared_), id(id_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(rule_iterator);
        }

        explicit DotItem(
            TextIterator &&rule_iterator_,
            size_t id_,
            AnalysisShared &analysis_shared_)
          : BasicItem(analysis_shared_), id(id_)
        {
            parseRule(rule_iterator_);
        }

        explicit DotItem(TextIterator &rule_iterator_, size_t id_, AnalysisShared &analysis_shared_)
          : BasicItem(analysis_shared_), id(id_)
        {
            parseRule(rule_iterator_);
        }

        [[nodiscard]] auto getId() const -> size_t
        {
            return id;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return items.empty();
        }

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator) const -> bool override;

        auto parseRule(TextIterator &rule_iterator) -> void;

        [[nodiscard]] static auto movedToTheNextChar(TextIterator &rule_iterator) -> bool
        {
            return not isEoF(rule_iterator.nextRawChar());
        }

        auto recognizeAction(TextIterator &rule_iterator, char32_t chr, ItemsCounter &counter)
            -> void;

        [[nodiscard]] auto constructNewSequence(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto constructNewUnion(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto constructNewItem(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem>;

        auto emplaceItem(std::unique_ptr<BasicItem> item) -> void;

        auto constructString(TextIterator &rule_iterator, bool is_character, bool is_multiline)
            -> void;

        auto constructTerminal(TextIterator &rule_iterator) -> void;

        auto addRepetition(TextIterator &rule_iterator, Repetition new_repetition) -> void;

        auto reverseLastItem(TextIterator &rule_iterator) -> void;

        auto checkSize(
            TextIterator &rule_iterator, size_t expected_size, u8string_view message,
            u8string_view suggestion = {}) -> void;

        static auto throwUnexpectedSize(
            TextIterator &rule_iterator,
            u8string_view message,
            u8string_view suggestion = {}) -> void;

        static auto throwUnableToApply(
            TextIterator &rule_iterator,
            u8string_view reason,
            u8string_view suggestion = {}) -> void;

        static auto throwUnterminatedDotItem(TextIterator &rule_iterator) -> void;

        static auto throwUndefinedAction(TextIterator &rule_iterator) -> void;

        boost::container::small_vector<std::unique_ptr<BasicItem>, 4> items{};
        size_t id{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
