#include <ccl/lexer/rule/binary_expression/binary_operation_or.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleAlternative::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *lhs_node = front().get();
        const auto *rhs_node = back().get();

        auto lhs = static_cast<const RulesReaderNode *>(lhs_node)->construct(parser_builder);
        auto rhs = static_cast<const RulesReaderNode *>(rhs_node)->construct(parser_builder);

        auto lhs_as_rule = isl::get<std::unique_ptr<lexer::rule::Container>>(lhs);
        auto rhs_as_rule = isl::get<std::unique_ptr<lexer::rule::RuleBlockInterface>>(rhs);

        auto result = std::unique_ptr<lexer::rule::RuleBlockInterface>{
            std::make_unique<lexer::rule::BinaryOperationOr>(
                lexer::rule::RuleBlock{std::move(lhs_as_rule)},
                lexer::rule::RuleBlock{std::move(rhs_as_rule)})};

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
