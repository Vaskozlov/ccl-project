#ifndef CCL_PROJECT_UNION_HPP
#define CCL_PROJECT_UNION_HPP

#include <ccl/lexer/rule/rule_block_interface.hpp>
#include <isl/utf_set.hpp>

namespace ccl::lexer::rule
{
    auto parseUnionDecl(isl::string_view union_decl)
        -> isl::Pair<std::bitset<isl::UtfSet::asciiStorageSize>, std::vector<isl::Range<char32_t>>>;

    class Union final : public RuleBlockInterface
    {
    private:
        isl::UtfSet storedSymbols;

    public:
        using typename RuleBlockInterface::TextIterator;

        [[nodiscard]] explicit Union(TextIterator &rule_iterator);

        [[nodiscard]] explicit Union(isl::UtfSet stored_symbols);

        [[nodiscard]] auto
            getStoredSymbols() const noexcept CCL_LIFETIMEBOUND -> const isl::UtfSet &
        {
            return storedSymbols;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return storedSymbols.empty();
        }

    private:
        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) noexcept -> bool;

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool;

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void;

        static auto checkForUnexpectedEnd(
            const TextIterator &rule_iterator, bool is_escaping, char32_t chr) -> void;

        static auto checkUnionBegin(const TextIterator &rule_iterator) -> void;

        static auto
            checkForClosedRange(const TextIterator &rule_iterator, bool is_ranged_opened) -> void;

        [[noreturn]] static auto throwUnterminatedUnion(const TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto
            throwUnionBeginException(const TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto
            throwUnterminatedRangeException(const TextIterator &rule_iterator) -> void;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_UNION_HPP */
