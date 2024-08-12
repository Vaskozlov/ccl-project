#include <ccl/parser/rules_reader/ast/any_block.hpp>

namespace ccl::parser::reader::ast
{
    auto AnyBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        for (const auto &node : this->nodes) {
            const auto *elem = dynamic_cast<RulesReaderNode *>(node.get());

            if (elem != nullptr) {
                elem->construct(parser_builder);
            }
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast