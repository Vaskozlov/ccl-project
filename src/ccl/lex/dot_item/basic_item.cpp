#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    auto
        BasicItem::scan(const TextIterator &text_iterator, const Token &token, bool main_scan) const
        -> std::optional<std::pair<TextIterator, Token>>
    {
        auto times = static_cast<size_t>(0);
        auto local_iterator = text_iterator;
        auto local_token = Token{ token };

        while (times < recurrence.to) {
            if (not local_iterator.isEnd() && scanIterationCall(local_iterator, local_token)) {
                ++times;
            } else {
                break;
            }
        }

        if (successfullyScanned(local_iterator, times, main_scan)) {
            if (times != 0) {
                modifyToken(text_iterator, local_iterator, local_token);
            }

            return std::optional<std::pair<TextIterator, Token>>(
                std::in_place, local_iterator, local_token);
        }

        return std::nullopt;
    }

    auto BasicItem::scanIterationCall(TextIterator &local_iterator, Token &local_token) const
        -> bool
    {
        auto token_copy = local_token;
        auto iterator_copy = local_iterator;

        if (scanIteration(iterator_copy, token_copy) ^ reversed) {
            local_iterator = std::move(iterator_copy);
            local_token = std::move(token_copy);
            return true;
        }

        return false;
    }

    auto BasicItem::successfullyScanned(
        const TextIterator &text_iterator, size_t times, bool main_scan) const -> bool
    {
        return recurrence.inRange(times) &&
               (not main_scan || analysis_shared.isNextCharNotForScanning(text_iterator));
    }

    auto BasicItem::modifyToken(
        const TextIterator &before_scan_iterator, const TextIterator &after_scan_iterator,
        Token &token) const -> void
    {
        const auto *new_token_end = after_scan_iterator.getRemainingAsCarriage();
        token.setEnd(new_token_end);

        if (hasPrefix()) {
            token.addPrefix({ before_scan_iterator.getRemainingAsCarriage(), new_token_end });
        } else if (hasPostfix()) {
            token.addPostfix({ before_scan_iterator.getRemainingAsCarriage(), new_token_end });
        }
    }
}// namespace ccl::lex::dot_item
