#ifndef CCL_PROJECT_TOKEN_HPP
#define CCL_PROJECT_TOKEN_HPP

#include <ccl/text/text_iterator.hpp>

namespace ccl::lexer
{
    // NOLINTNEXTLINE
    enum struct ReservedTokenType : SmallId
    {
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2
    };

    constexpr inline SmallId ReservedTokenMaxValue = 2;

    namespace rule
    {
        class Container;
    }

    class Token
    {
    private:
        friend class LexicalAnalyzer;
        friend class rule::Container;

        std::vector<isl::string_view> extractedParts;
        isl::string_view repr;
        text::Location location;
        const text::InputInfo *inputInfo{};
        SmallId id{};

    public:
        Token() = default;

        [[nodiscard]] explicit Token(SmallId token_id, const text::InputInfo *input_info);

        [[nodiscard]] Token(
            SmallId token_id, isl::string_view token_repr, text::Location token_location,
            const text::InputInfo *input_info);

        [[nodiscard]] Token(SmallId token_id, const text::TextIterator &text_iterator);

        [[nodiscard]] auto getId() const noexcept -> SmallId
        {
            return id;
        }

        [[nodiscard]] auto isEmptyRepr() const noexcept -> bool
        {
            return getReprSize() != 0;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return getId() != 0;
        }

        [[nodiscard]] auto getReprSize() const noexcept -> std::size_t
        {
            return repr.size();
        }

        [[nodiscard]] auto getLocation() const noexcept CCL_LIFETIMEBOUND -> const text::Location &
        {
            return location;
        }

        [[nodiscard]] auto getLine() const noexcept -> u32
        {
            return location.line;
        }

        [[nodiscard]] auto getColumn() const noexcept -> u32
        {
            return location.column;
        }

        [[nodiscard]] auto getRealColumn() const -> u32;

        [[nodiscard]] auto getFilename() const noexcept CCL_LIFETIMEBOUND -> isl::string_view
        {
            return inputInfo->filename;
        }

        [[nodiscard]] auto getRepr() const noexcept CCL_LIFETIMEBOUND -> isl::string_view
        {
            return repr;
        }

        [[nodiscard]] auto getExtractedParts() const noexcept CCL_LIFETIMEBOUND -> const auto &
        {
            return extractedParts;
        }

        [[nodiscard]] auto getInlineRepr() const noexcept -> isl::string_view;

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> isl::string_view;

        [[nodiscard]] auto cut(std::size_t first, std::size_t length) const -> Token;

    private:
        auto clear(SmallId new_id) noexcept -> void;

        auto setReprLength(std::size_t length) noexcept -> void
        {
            repr = repr.changeLength<isl::FunctionAPI::UNSAFE>(length);
        }

        auto addExtractedPart(isl::string_view prefix) -> void
        {
            extractedParts.push_back(prefix);
        }

        CCL_INLINE auto setEnd(isl::string_view::iterator end_of_repr) noexcept -> void
        {
            repr = {repr.begin(), end_of_repr};
        }

        auto finishInitialization(text::TextIterator &text_iterator, std::size_t totally_skipped)
            -> void;
    };

    CCL_DECL auto isUserDefinedTokenOrEoF(SmallId token_id) -> bool
    {
        return token_id == std::to_underlying(ReservedTokenType::EOI)
                   ? true
                   : token_id > ReservedTokenMaxValue;
    }
}// namespace ccl::lexer

#endif /* CCL_PROJECT_TOKEN_HPP */
