#ifndef CCL_PROJECT_UNION_HPP
#define CCL_PROJECT_UNION_HPP

#include <ccl/lex/dot_item/basic_item.hpp>
#include <ccl/utf_set.hpp>

namespace ccl::lex::dot_item
{
    class Union final : public BasicItem
    {
    public:
        using typename BasicItem::TextIterator;

        Union(TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_ = 0);

        [[nodiscard]] auto getBitset() const noexcept -> const UtfSet &
        {
            return bitset;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final;

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token & /* unused */) const
            -> bool final;

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) noexcept -> bool;

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool;

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void;

        static auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr)
                -> void;

        static auto checkUnionBegin(TextIterator &rule_iterator) -> void;
        static auto checkForClosedRange(TextIterator &rule_iterator, bool is_ranged_opened) -> void;

        static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void;
        static auto throwUnterminatedRangeException(TextIterator &rule_iterator) -> void;
        static auto throwUnionBeginException(TextIterator &rule_iterator) -> void;

        UtfSet bitset{};
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_UNION_HPP */
