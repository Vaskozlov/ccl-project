#ifndef CCL_PROJECT_PARSER_RULE_DECL_HPP
#define CCL_PROJECT_PARSER_RULE_DECL_HPP

#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleDecl final : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        [[nodiscard]] auto getDeclarationName() const -> isl::string_view;

        [[nodiscard]] auto getAlternatives() const -> const ParserRuleAlternatives *;

        [[nodiscard]] auto
            construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_DECL_HPP */
