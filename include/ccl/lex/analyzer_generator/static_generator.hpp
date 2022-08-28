#ifndef CCL_PROJECT_STATIC_GENERATOR_HPP
#define CCL_PROJECT_STATIC_GENERATOR_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <map>
#include <stack>

namespace ccl::lex::gen
{
    class StaticGenerator
    {
    public:
        using Tokenizer = typename LexicalAnalyzer::Tokenizer;

        explicit StaticGenerator(Tokenizer &tokenizer_) : tokenizer{ tokenizer_ }
        {
            generateHeader();
        }

    private:
        auto generateHeader() -> void;

        std::map<std::u8string, std::u8string> rules{};
        std::stack<Token> token_stack{};
        std::u8string generated_header{};
        Tokenizer &tokenizer;
    };
}// namespace ccl::lex::gen

#endif /* CCL_PROJECT_STATIC_GENERATOR_HPP */
