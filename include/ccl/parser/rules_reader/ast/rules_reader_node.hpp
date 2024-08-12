#ifndef CCL_PROJECT_RULES_READER_NODE_HPP
#define CCL_PROJECT_RULES_READER_NODE_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>
#include <isl/unique_any.hpp>

namespace ccl::parser::reader::ast
{
    class RulesReaderNode : public parser::ast::UnNodeSequence
    {
    public:
        using parser::ast::UnNodeSequence::NodeSequence;

        virtual auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny = 0;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_RULES_READER_NODE_HPP */
