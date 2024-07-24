#ifndef CCL_PROJECT_RULES_READER_NODE_HPP
#define CCL_PROJECT_RULES_READER_NODE_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>
#include <isl/unique_any.hpp>

namespace ccl::parser::reader::ast
{
    class RulesReaderNode
    {
    public:
        RulesReaderNode() = default;

        RulesReaderNode(const RulesReaderNode &) = delete;
        RulesReaderNode(RulesReaderNode &&) noexcept = default;

        virtual ~RulesReaderNode() = default;

        auto operator=(const RulesReaderNode &) -> void = delete;
        auto operator=(RulesReaderNode &&) -> void = delete;

        virtual auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny = 0;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_RULES_READER_NODE_HPP */
