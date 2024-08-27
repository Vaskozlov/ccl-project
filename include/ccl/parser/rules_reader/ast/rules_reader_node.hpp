#ifndef CCL_PROJECT_RULES_READER_NODE_HPP
#define CCL_PROJECT_RULES_READER_NODE_HPP

#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>
#include <isl/unique_any.hpp>

namespace ccl::parser::reader::ast
{
    class RulesReaderNode : public parser::ast::NodeOfNodes
    {
    private:
        using ConstructionFunction = Rule::RuleBuilderFunction;
        using ConversionTable = isl::StaticFlatmap<SmallId, ConstructionFunction, 15>;

    public:
        using NodeOfNodes::NodeOfNodes;

        virtual auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny = 0;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_RULES_READER_NODE_HPP */
