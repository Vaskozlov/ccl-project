#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleAlternatives::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *lhs_node = static_cast<const RulesReaderNode *>(front().get());
        auto lhs_result = lhs_node->construct(parser_builder);

        if (size() == 1) {
            auto lhs_alternative = lhs_result.release<std::vector<Symbol>>();
            return isl::UniqueAny{std::vector<std::vector<Symbol>>{std::move(*lhs_alternative)}};
        }

        const auto *rhs_node = static_cast<const RulesReaderNode *>(back().get());
        auto rhs_result = rhs_node->construct(parser_builder);
        auto rhs_alternative = rhs_result.release<std::vector<Symbol>>();

        auto result = isl::get<std::vector<std::vector<Symbol>>>(std::move(lhs_result));
        result.emplace_back(std::move(*rhs_alternative));

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
