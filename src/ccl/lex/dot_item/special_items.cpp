#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    auto BasicItem::SpecialItems::scan(const TextIterator &text_iterator) const -> ScanResult
    {
        for (auto &&item : special_items) {
            auto scan_result = item->scan(text_iterator, ScanType::SPECIAL);

            if (scan_result.has_value() && not scan_result->second.getRepr().empty()) {
                return scan_result;
            }
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
