#ifndef CCL_PROJECT_RULES_READER_NODE_HPP
#define CCL_PROJECT_RULES_READER_NODE_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>
#include <isl/unique_any.hpp>

namespace ccl::parser::reader::ast
{
    class RulesReaderNode : public parser::ast::UnNodeSequence
    {
    public:
        using parser::ast::UnNodeSequence::NodeSequence;

        virtual auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny = 0;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_RULES_READER_NODE_HPP */
