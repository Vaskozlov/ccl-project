#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/tokenizer.hpp>

namespace ccl::lexer
{
    LexicalAnalyzer::LexicalAnalyzer(const ExceptionHandler &exception_handler)
      : exceptionHandler{std::addressof(exception_handler)}
    {}

    LexicalAnalyzer::LexicalAnalyzer(
        const ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
        const isl::string_view filename, std::vector<SmallId> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{std::addressof(exception_handler)}
    {
        for (const auto &rule : rules) {
            createContainer(rule, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(
        const Rule &rule, const u32 id, const isl::string_view filename) -> void
    {
        auto container = std::make_unique<Container>(
            *this, TextIterator{rule.repr, *exceptionHandler, filename}, true);

        container->setId(id);
        addContainer(rule.name, std::move(container));
    }

    auto LexicalAnalyzer::addContainer(
        const std::string &rule_name, std::unique_ptr<Container> new_container) -> void
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

    auto LexicalAnalyzer::getTokenizer(isl::string_view text, isl::string_view filename) const
        -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        isl::string_view text, isl::string_view filename,
        ExceptionHandler &handler) const -> Tokenizer
    {
        return {*this, text, filename, handler};
    }
}// namespace ccl::lexer
