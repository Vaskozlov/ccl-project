#ifndef CCL_PROJECT_PARSER_RULE_BODY_HPP
#define CCL_PROJECT_PARSER_RULE_BODY_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleBody : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;

    private:
        auto addStringToLexicalAnalyzer(
            isl::string_view str, ParserBuilder &parser_builder,
            bool is_any_place_string) const -> void;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_BODY_HPP */
