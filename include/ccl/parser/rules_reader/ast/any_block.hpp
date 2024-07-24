#ifndef CCL_PROJECT_ANY_BLOCK_HPP
#define CCL_PROJECT_ANY_BLOCK_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class AnyBlock
      : public parser::ast::NodeSequence<isl::UniquePtr>
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_ANY_BLOCK_HPP */
