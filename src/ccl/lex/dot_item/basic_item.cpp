#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    auto BasicItem::alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) {
            text_iterator.throwSuggestion("item will be always recognized", "delete it");
        }
    }

    auto BasicItem::neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) {
            text_iterator.throwSuggestion("item will never be recognized");
        }
    }

    auto BasicItem::scan(const TextIterator &text_iterator, const Token &token, ScanType scan_type)
        const -> std::optional<std::pair<TextIterator, Token>>
    {
        auto times = static_cast<size_t>(0);
        auto local_iterator = text_iterator;
        auto local_token = Token{ token };

        if (scan_type != ScanType::SPECIAL) {
            auto scan_result = special_items.scan(local_iterator);

            if (scan_result.has_value()) {
                return *scan_result;
            }
        }

        while (times < recurrence.to) {
            if (not local_iterator.isEnd() && scanIterationCall(local_iterator, local_token)) {
                ++times;
            } else {
                break;
            }
        }

        if (successfullyScanned(local_iterator, times, scan_type == ScanType::MAIN)) {
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
               (not main_scan || isLayoutOrEoF(text_iterator.getNextCarriageValue()));
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
