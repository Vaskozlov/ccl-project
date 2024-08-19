#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        for (auto *node : this->nodes) {
            static_cast<RulesReaderNode *>(node)->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
