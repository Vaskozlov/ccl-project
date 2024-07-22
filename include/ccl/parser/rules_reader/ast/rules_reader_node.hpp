#ifndef CCL_PROJECT_RULES_READER_NODE_HPP
#define CCL_PROJECT_RULES_READER_NODE_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
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

        virtual auto
            construct(lexer::LexicalAnalyzer &lexical_analyzer) const -> isl::UniqueAny = 0;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_RULES_READER_NODE_HPP */
