#ifndef CERBERUS_PROJECT_UNION_HPP
#define CERBERUS_PROJECT_UNION_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <cerberus/utf_set.hpp>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(UnionException, text::TextIteratorException);

    class Union : public BasicItem
    {
    public:
        using BasicItem::isNextCharNotForScanning;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::ScanStatus;
        using typename BasicItem::TextIterator;

        Union(TextIterator &rule_iterator_, AnalysisShared &analysis_shared_);

        [[nodiscard]] auto get() const noexcept -> const UtfSet &
        {
            return bitset;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override;

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token & /* unused */) const
            -> bool override;

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) noexcept -> bool;

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool;

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void;

        static auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr)
                -> void;

        static auto checkUnionBegin(TextIterator &rule_iterator) -> void;
        static auto checkForClosedRange(TextIterator &rule_iterator, bool is_open) -> void;

        static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void;
        static auto throwUnterminatedRangeException(TextIterator &rule_iterator) -> void;
        static auto throwUnionBeginException(TextIterator &rule_iterator) -> void;

        UtfSet bitset{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_UNION_HPP */
