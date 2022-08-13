#include <cerberus/lex/dot_item/basic_item.hpp>

namespace cerb::lex::dot_item
{
    auto BasicItem::isNextCharNotForScanning(const TextIterator &text_iterator) const -> bool
    {
        auto chr = text_iterator.futureRawChar(1);

        if (isLayoutOrEoF(chr)) {
            return true;
        }

        return analysis_shared.isNextCharNotForScanning(text_iterator);
    }

    auto BasicItem::scan(const TextIterator &text_iterator, bool main_scan) const
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

    auto BasicItem::computeScanResult(
        const TextIterator &text_iterator, size_t times, bool main_scan) const -> bool
    {
        return repetition.inRange(times) &&
               (not main_scan || isNextCharNotForScanning(text_iterator));
    }
}// namespace cerb::lex::dot_item
