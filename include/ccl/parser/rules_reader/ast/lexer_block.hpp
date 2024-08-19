#ifndef CCL_PROJECT_LEXER_BLOCK_HPP
#define CCL_PROJECT_LEXER_BLOCK_HPP

#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerBlock : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_BLOCK_HPP */
