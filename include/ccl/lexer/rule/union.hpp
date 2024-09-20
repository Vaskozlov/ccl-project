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
        using RuleBlockInterface::TextIterator;

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

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_UNION_HPP */
