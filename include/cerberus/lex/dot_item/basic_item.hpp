#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/repetition.hpp>
#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <expected>

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

        CERBLIB_DECL auto isNextCharNotForScanning(const TextIterator &text_iterator) const -> bool
        {
            auto chr = text_iterator.futureRawChar(1);

            if (lor(isLayout(chr), isEoF(chr))) {
                return true;
            }

            return not analysis_shared.isNextCharForScanning(text_iterator);
        }

        CERBLIB_DECL virtual auto scan(const TextIterator &text_iterator) const
            -> std::pair<bool, TextIterator> = 0;

        CERBLIB_DECL auto scan2(const TextIterator &text_iterator) const
            -> std::pair<bool, TextIterator>
        {
            auto local_iterator = text_iterator;
            auto times = static_cast<size_t>(0);

            while (times <= repetition.to) {
                if (isNextCharNotForScanning(local_iterator)) {
                    break;
                }

                if (auto [success, iterator] = scan(local_iterator); success) {
                    ++times;
                    local_iterator = iterator;
                } else {
                    break;
                }
            }

            return { repetition.inRange(times), local_iterator };
        }

        auto operator=(const BasicItem &) -> BasicItem & = default;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = default;

        constexpr explicit BasicItem(AnalysisShared<CharT> &analysis_shared_)
          : analysis_shared{ analysis_shared_ }
        {}

        BasicItem() = default;
        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;

    protected:
        Repetition repetition{ Repetition::basic() };
        AnalysisShared<CharT> &analysis_shared;
        bool reversed{ false };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
