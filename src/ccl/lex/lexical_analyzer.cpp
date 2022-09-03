#include <ccl/lex/lexical_analyzer.hpp>
#include <utility>

namespace ccl::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler_, const std::initializer_list<Rule> &rules_,
        string_view filename, std::basic_string<size_t> ignored_ids_)
      : ignored_ids(std::move(ignored_ids_)), exception_handler(exception_handler_)
    {
        for (const Rule &rule : rules_) {
            createContainer(rule.repr, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(string_view rule, size_t id, string_view filename) -> void
    {
        auto container =
            Container(TextIterator{ rule, exception_handler, filename }, id, special_items);

        if (container.isSpecial()) {
            special_items.special_items.emplace_back(
                std::make_unique<Container>(std::move(container)));
        } else if (not container.empty()) {
            items.emplace(std::move(container));
        }
    }

}// namespace ccl::lex
