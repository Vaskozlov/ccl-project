#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/repetition.hpp>
#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator<CharT>;
        using CommentTokens = text::module::CommentTokens<CharT>;
        using ExceptionAccumulator =
            analysis::ExceptionAccumulator<text::TextIteratorException<CharT>>;

        enum struct ScanStatus : bool
        {
            FAILURE = false,
            SUCCESS = true
        };

        CERBLIB_DECL auto getRepetition() const -> Repetition
        {
            return repetition;
        }

        CERBLIB_DECL auto isReversed() const -> bool
        {
            return reversed;
        }

        constexpr auto makeReverse() -> void
        {
            reversed = true;
        }

        constexpr auto setRepetition(Repetition new_repetition) -> void
        {
            repetition = new_repetition;
        }

        CERBLIB_DECL auto
            isNextCharForScanning(TextIterator &text_iterator, AnalysisShared<CharT> &shared) const
            -> bool
        {
            auto chr = text_iterator.futureRawChar();

            if (lor(isLayout(chr), isEoF(chr))) {
                return false;
            }

            auto text = text_iterator.getRemainingFuture(1);
        }

        CERBLIB_DECL virtual auto scan(TextIterator &text_iterator) const -> ScanStatus = 0;

        auto operator=(const BasicItem &) -> BasicItem & = default;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = default;

        BasicItem() = default;
        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;

    protected:
        Repetition repetition{ Repetition::basic() };
        bool reversed{ false };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
