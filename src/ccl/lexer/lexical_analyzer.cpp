#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/peg_parser.hpp>

namespace ccl::lexer
{
    LexicalAnalyzer::LexicalAnalyzer(ExceptionHandler &exception_handler)
      : exceptionHandler{exception_handler}
    {}

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
        isl::string_view filename, std::vector<u32> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{exception_handler}
    {
        for (const auto &rule : rules) {
            createContainer(rule, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(const Rule &rule, u32 id, isl::string_view filename)
        -> void
    {
        auto container = isl::makeUnique<Container>(
            *this, TextIterator{rule.repr, exceptionHandler, filename}, true);

        container->setId(id);
        addContainer(rule.name, std::move(container));
    }

    auto LexicalAnalyzer::addContainer(
        isl::string_view rule_name, isl::UniquePtr<Container> new_container) -> void
    {
        if (new_container->getId() <= ReservedTokenMaxValue) {
            throw UnrecoverableError{"Reserved token id is used for a custom token"};
        }

        allItemsMap.try_emplace(rule_name, new_container.get());

        if (new_container->isAnyPlaceItem()) {
            anyPlaceItems.items.emplace_back(std::move(new_container));
        } else {
            items.emplace_back(std::move(new_container));
        }
    }

    auto
        LexicalAnalyzer::getTokenizer(isl::string_view text, isl::string_view filename) -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        isl::string_view text, isl::string_view filename, ExceptionHandler &handler) -> Tokenizer
    {
        return {*this, text, filename, handler};
    }

    [[nodiscard]] auto LexicalAnalyzer::getParser(
        isl::string_view rule_name, isl::string_view text, isl::string_view filename,
        ExceptionHandler &handler) -> PegParser
    {
        return {rule_name, *this, text, filename, handler};
    }

    [[nodiscard]] auto LexicalAnalyzer::getParser(
        isl::string_view rule_name, isl::string_view text, isl::string_view filename) -> PegParser
    {
        return getParser(rule_name, text, filename, exceptionHandler);
    }
}// namespace ccl::lexer
