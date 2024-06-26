#ifndef CCL_PROJECT_UNION_HPP
#define CCL_PROJECT_UNION_HPP

#include <ccl/lexer/dot_item/item_concept.hpp>
#include <isl/utf_set.hpp>

namespace ccl::lexer::dot_item
{
    class Union final : public DotItemConcept
    {
    private:
        isl::UtfSet bitset;

    public:
        using typename DotItemConcept::TextIterator;

        [[nodiscard]] explicit Union(TextIterator &rule_iterator, Id rule_id = 0);

        [[nodiscard]] auto getBitset() const noexcept CCL_LIFETIMEBOUND -> const isl::UtfSet &
        {
            return bitset;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return bitset.empty();
        }

    private:
        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;

        [[nodiscard]] auto
            parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult override;

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) noexcept -> bool;

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool;

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void;

        static auto checkForUnexpectedEnd(
            TextIterator &rule_iterator, bool is_escaping, char32_t chr) -> void;

        static auto checkUnionBegin(TextIterator &rule_iterator) -> void;
        static auto checkForClosedRange(TextIterator &rule_iterator, bool is_ranged_opened) -> void;

        [[noreturn]] static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void;
        [[noreturn]] static auto throwUnionBeginException(TextIterator &rule_iterator) -> void;
        [[noreturn]] static auto
            throwUnterminatedRangeException(TextIterator &rule_iterator) -> void;
    };
}// namespace ccl::lexer::dot_item

#endif /* CCL_PROJECT_UNION_HPP */
