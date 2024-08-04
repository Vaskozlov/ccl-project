#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerBlock::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        for (const auto &node : this->nodes) {
            static_cast<RulesReaderNode *>(node.get())->construct(rule_constructor);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
