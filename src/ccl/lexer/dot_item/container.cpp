#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>

namespace ccl::lexer::rule
{
    CCL_INLINE auto Container::addExtractedPart(
        const RuleBlockInterface *item, Token &token, const isl::string_view repr) -> void
    {
        if (item->isExtractable()) [[unlikely]] {
            token.addExtractedPart(repr);
        }
    }

    Container::Container(
        LexicalAnalyzer &lexical_analyzer, TextIterator &rule_iterator, const bool main_item,
        const bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexicalAnalyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {
        parseRule(rule_iterator);
    }

    Container::Container(
        LexicalAnalyzer &lexical_analyzer, const TextIterator &rule_iterator, const bool main_item,
        const bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexical_analyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {
        auto text_iterator_copy = rule_iterator;
        parseRule(text_iterator_copy);
    }

    Container::Container(
        LexicalAnalyzer &lexical_analyzer, const bool main_item, const bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexical_analyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {}

    auto Container::beginScan(
        TextIterator &text_iterator, Token &token, const ScanType special_scan) const -> bool
    {
        auto totally_skipped = std::size_t{};
        auto local_iterator = text_iterator.fork();

        token.clear(getId());

        for (const RuleBlock &item : items) {
            auto scan_result = item->scan(local_iterator);

            if (scan_result.isFailure() && isReversed()) {
                scan_result = ScanResult{isl::utf8::size(local_iterator.getNextCarriageValue())};
            } else if (scan_result.isFailure()) {
                return false;
            }

            const auto prefix_or_postfix_repr = isl::string_view{
                local_iterator.getRemainingAsCarriage(), scan_result.getBytesCount()};

            addExtractedPart(item.get(), token, prefix_or_postfix_repr);

            totally_skipped += scan_result.getBytesCount();
            local_iterator.skip(scan_result.getBytesCount());
        }

        if (special_scan == ScanType::BASIC && !itemSuccessfullyEnded(local_iterator)) {
            return false;
        }

        token.finishInitialization(text_iterator, totally_skipped);
        return true;
    }

    CCL_INLINE auto
        Container::itemSuccessfullyEnded(const ForkedGenerator &text_iterator) const -> bool
    {
        return isLayoutOrEoF(text_iterator.getNextCarriageValue()) ||
               anyPlaceItems.checkForSpecial(text_iterator);
    }

    auto Container::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        auto totally_skipped = std::size_t{};
        auto local_iterator = text_iterator;

        for (const RuleBlock &item : items) {
            auto scan_result = item->scan(local_iterator);

            if (scan_result.isFailure()) {
                return ScanResult::failure();
            }

            totally_skipped += scan_result.getBytesCount();
            local_iterator.skip(scan_result.getBytesCount());
        }

        return ScanResult{totally_skipped};
    }

    auto Container::parseRule(TextIterator &rule_iterator) -> void
    {
        std::ignore = RuleParser{*this, rule_iterator};
    }
}// namespace ccl::lexer::rule
