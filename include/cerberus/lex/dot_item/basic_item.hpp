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

        constexpr auto reverse() -> void
        {
            reversed = !reversed;
        }

        constexpr auto setRepetition(Repetition new_repetition) -> void
        {
            repetition = new_repetition;
        }

        CERBLIB_DECL auto isNextCharNotForScanning(const TextIterator &text_iterator) const -> bool
        {
            auto chr = text_iterator.futureRawChar(1);

            if (isLayoutOrEoF(chr)) {
                return true;
            }

            return analysis_shared.isNextCharNotForScanning(text_iterator);
        }

        CERBLIB_DECL auto canBeOptimized() const -> bool
        {
            return empty() && repetition.from == 0;
        }

        CERBLIB_DECL virtual auto empty() const noexcept -> bool = 0;

        CERBLIB_DECL auto scan(const TextIterator &text_iterator, bool main_scan = false) const
            -> std::pair<bool, TextIterator>
        {
            auto times = static_cast<size_t>(0);
            auto local_iterator = text_iterator;

            while (times <= repetition.to) {
                auto iterator_copy = local_iterator;

                if (scanIteration(iterator_copy) ^ reversed) {
                    ++times;
                    local_iterator = std::move(iterator_copy);
                } else {
                    break;
                }
            }

            return { computeScanResult(local_iterator, times, main_scan), local_iterator };
        }

    private:
        CERBLIB_DECL virtual auto scanIteration(TextIterator &text_iterator) const -> bool = 0;

        CERBLIB_DECL auto
            computeScanResult(const TextIterator &text_iterator, size_t times, bool main_scan) const
            -> bool
        {
            return repetition.inRange(times) &&
                   (not main_scan || isNextCharNotForScanning(text_iterator));
        }

    public:
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
