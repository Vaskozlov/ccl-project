#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/string_node.hpp>

namespace ccl::lexer::rule
{
    CCL_INLINE auto Container::addPrefixOrPostfix(
        const RuleBlockInterface *item, Token &token, isl::string_view repr) -> void
    {
        if (repr.empty()) [[unlikely]] {
            return;
        }

        if (item->hasPrefix()) [[unlikely]] {
            token.addPrefix(repr);
        } else if (item->hasPostfix()) [[unlikely]] {
            token.addPostfix(repr);
        }
    }

    Container::Container(
        LexicalAnalyzer &lexical_analyzer, TextIterator &rule_iterator, bool main_item,
        bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexicalAnalyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {
        parseRule(rule_iterator);
    }

    Container::Container(
        LexicalAnalyzer &lexical_analyzer, const TextIterator &rule_iterator, bool main_item,
        bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexical_analyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {
        auto text_iterator_copy = rule_iterator;
        parseRule(text_iterator_copy);
    }

    Container::Container(LexicalAnalyzer &lexical_analyzer, bool main_item, bool is_special)
      : lexicalAnalyzer{lexical_analyzer}
      , anyPlaceItems{lexical_analyzer.shareAnyPlaceItems()}
      , flags{.isMain = main_item, .isAnyPlace = is_special}
    {}

    auto Container::beginScan(
        TextIterator &text_iterator, Token &token, ScanType special_scan) const -> bool
    {
        auto totally_skipped = isl::as<std::size_t>(0);
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

            addPrefixOrPostfix(item.get(), token, prefix_or_postfix_repr);

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
        auto totally_skipped = isl::as<std::size_t>(0);
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

    auto Container::parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult
    {
        auto totally_skipped = isl::as<std::size_t>(0);
        auto local_iterator = text_iterator;
        auto node_sequence = isl::makeUnique<parser::ast::UnNodeSequence>(getId());

        for (const RuleBlock &item : items) {
            auto parsing_result = item->parse(local_iterator);

            if (parsing_result.isFailure()) {
                return ParsingResult::failure();
            }

            const auto is_hidden_item = item->getFlags().isHiddenFromParser;

            if (parsing_result.getNode() == nullptr && !is_hidden_item) {
                node_sequence->addNode(isl::makeUnique<parser::ast::StringNode>(
                    getId(),
                    local_iterator.getRemaining().substr(0, parsing_result.getBytesCount())));
            } else if (!is_hidden_item) {
                if (isl::is<RuleReference *>(item.get())) {
                    node_sequence->addNode(parsing_result.getAndReleaseNode());
                } else {
                    node_sequence->joinWithNode(parsing_result.getAndReleaseNode());
                }
            }

            totally_skipped += parsing_result.getBytesCount();
            local_iterator.skip(parsing_result.getBytesCount());
        }

        return ParsingResult{totally_skipped, std::move(node_sequence)};
    }

    auto Container::parseRule(TextIterator &rule_iterator) -> void
    {
        // TODO: use function instead
        RuleParser{*this, rule_iterator};
    }
}// namespace ccl::lexer::rule
