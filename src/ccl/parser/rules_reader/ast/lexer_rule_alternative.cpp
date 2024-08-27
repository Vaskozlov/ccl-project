#include <ccl/lexer/rule/binary_expression/binary_operation_or.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleAlternative::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *lhs_node = front().get();
        auto lhs = static_cast<const RulesReaderNode *>(lhs_node)->construct(parser_builder);

        if (size() == 1) {
            return lhs;
        }

        auto lhs_as_rule = isl::get<std::unique_ptr<lexer::rule::Container>>(lhs);

        const auto *rhs_node = back().get();
        auto rhs = static_cast<const RulesReaderNode *>(rhs_node)->construct(parser_builder);
        auto rhs_as_rule = isl::get<std::unique_ptr<lexer::rule::Container>>(rhs);

        auto alternative = lexer::rule::RuleBlock{
            std::make_unique<lexer::rule::BinaryOperationOr>(
                std::move(lhs_as_rule), std::move(rhs_as_rule)),
        };

        auto result = std::make_unique<lexer::rule::Container>(parser_builder.getLexicalAnalyzer());
        result->addItem(std::move(alternative));

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
