#ifndef CCL_PROJECT_PARSER_RULE_BODY_HPP
#define CCL_PROJECT_PARSER_RULE_BODY_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleBody
      : public parser::ast::UnNodeSequence
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;

    private:
        auto addStringToLexicalAnalyzer(
            isl::string_view str, RulesConstructor &rule_constructor) const -> void;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_BODY_HPP */
