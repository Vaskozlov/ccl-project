#ifndef CCL_PROJECT_PARSER_BLOCK_HPP
#define CCL_PROJECT_PARSER_BLOCK_HPP

#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    class ParserBlock final : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_BLOCK_HPP */
