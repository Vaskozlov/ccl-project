#include <ccl/parser/rules_reader/ast/blocks.hpp>

namespace ccl::parser::reader::ast
{
    auto Blocks::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *lexer_or_parser_block = static_cast<const RulesReaderNode *>(at(1).get());
        lexer_or_parser_block->construct(parser_builder);

        if (size() == 3) {
            const auto *next_block = static_cast<const RulesReaderNode *>(back().get());
            next_block->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast