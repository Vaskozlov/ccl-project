#ifndef CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP
#define CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/dot_item/dot_item.hpp>
#include <cerberus/lex/typedefs.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    class LexicalAnalyzer
    {
    public:
        constexpr LexicalAnalyzer(
            const std::initializer_list<std::pair<ssize_t, StrView<CharT>>> &rules_)
        {
            for (auto [id, rule] : rules_) {
                dot_item::DotItem(rule, id);
            }
        }

    private:
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP */
