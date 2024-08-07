#ifndef CCL_PROJECT_PARSER_BLOCK_HPP
#define CCL_PROJECT_PARSER_BLOCK_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>

namespace ccl::parser::reader::ast
{
    class ParserBlock : public LexerBlock
    {
    public:
        using LexerBlock::LexerBlock;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_BLOCK_HPP */
