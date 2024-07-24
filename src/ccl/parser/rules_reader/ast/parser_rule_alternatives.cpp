#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleAlternatives::construct(RulesConstructor &rule_constructor) const
        -> isl::UniqueAny
    {
        auto result = isl::Vector<isl::UniquePtr<isl::Vector<Id>>>();

        for (const auto &node : this->nodes) {
            const auto *alternative = dynamic_cast<const RulesReaderNode *>(node.get());
            result.emplace_back(
                isl::anyCast<isl::Vector<Id>>(alternative->construct(rule_constructor)));
        }

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast