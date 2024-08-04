#include <ccl/lexer/rule/binary_expression/binary_operation_or.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleAlternative::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        const auto *lhs_node = this->front();
        const auto *rhs_node = this->back();

        auto lhs = static_cast<const RulesReaderNode *>(lhs_node)->construct(rule_constructor);
        auto rhs = static_cast<const RulesReaderNode *>(rhs_node)->construct(rule_constructor);

        auto lhs_as_rule = isl::get<isl::UniquePtr<lexer::rule::Container>>(lhs);
        auto rhs_as_rule = isl::get<isl::UniquePtr<lexer::rule::RuleBlockInterface>>(rhs);

        auto result =
            isl::makeUnique<lexer::rule::RuleBlockInterface, lexer::rule::BinaryOperationOr>(
                lexer::rule::RuleBlock{std::move(lhs_as_rule)},
                lexer::rule::RuleBlock{std::move(rhs_as_rule)}, SmallId{});

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
