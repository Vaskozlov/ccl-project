#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        for (const auto &node : nodes) {
            static_cast<const RulesReaderNode *>(node.get())->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
