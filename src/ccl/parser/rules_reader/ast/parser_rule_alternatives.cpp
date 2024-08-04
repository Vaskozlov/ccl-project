#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleAlternatives::construct(RulesConstructor &rule_constructor) const
        -> isl::UniqueAny
    {
        auto result = std::vector<std::vector<SmallId>>();

        for (const auto &node : this->nodes) {
            const auto *alternative = static_cast<const RulesReaderNode *>(node.get());
            result.emplace_back(
                isl::get<std::vector<SmallId>>(alternative->construct(rule_constructor)));
        }

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast