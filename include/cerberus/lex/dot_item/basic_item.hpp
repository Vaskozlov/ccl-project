#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/repetition.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using CommentTokens = text::CommentTokens;
        using ExceptionAccumulator = analysis::ExceptionAccumulator<text::TextIteratorException>;

        enum struct ScanStatus : bool
        {
            FAILURE = false,
            SUCCESS = true
        };

        [[nodiscard]] auto getRepetition() const -> Repetition
        {
            return repetition;
        }

        [[nodiscard]] auto isReversed() const -> bool
        {
            return reversed;
        }

        auto reverse() -> void
        {
            reversed = !reversed;
        }

        auto setRepetition(Repetition new_repetition) -> void
        {
            repetition = new_repetition;
        }

        [[nodiscard]] auto isNextCharNotForScanning(const TextIterator &text_iterator) const -> bool
        {
            auto chr = text_iterator.futureRawChar(1);

            if (isLayoutOrEoF(chr)) {
                return true;
            }

            return analysis_shared.isNextCharNotForScanning(text_iterator);
        }

        [[nodiscard]] auto canBeOptimized() const -> bool
        {
            return empty() && repetition.from == 0;
        }

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] auto scan(const TextIterator &text_iterator, bool main_scan = false) const
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
        [[nodiscard]] virtual auto scanIteration(TextIterator &text_iterator) const -> bool = 0;

        [[nodiscard]] auto
            computeScanResult(const TextIterator &text_iterator, size_t times, bool main_scan) const
            -> bool
        {
            return repetition.inRange(times) &&
                   (not main_scan || isNextCharNotForScanning(text_iterator));
        }

    public:
        auto operator=(const BasicItem &) -> BasicItem & = delete;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = delete;

        explicit BasicItem(AnalysisShared &analysis_shared_) : analysis_shared{ analysis_shared_ }
        {}

        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;

    protected:
        Repetition repetition{ Repetition::basic() };
        AnalysisShared &analysis_shared;
        bool reversed{ false };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
