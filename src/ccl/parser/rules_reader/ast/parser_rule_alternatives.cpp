#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleAlternatives::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        auto result = std::vector<std::vector<SmallId>>();

        for (const auto &node : nodes) {
            const auto *alternative = static_cast<const RulesReaderNode *>(node.get());
            result.emplace_back(
                isl::get<std::vector<SmallId>>(alternative->construct(parser_builder)));
        }

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
