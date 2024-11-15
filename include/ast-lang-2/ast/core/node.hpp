#ifndef ASTLANG_2_NODE_HPP
#define ASTLANG_2_NODE_HPP

#include <ast-lang-2/ts/type.hpp>
#include <ccl/parser/ast/non_terminal.hpp>
#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>

namespace astlang2::interpreter
{
    class Interpreter;
}

namespace astlang2::ast::core
{
    template<std::derived_from<ccl::parser::ast::Node> T = ccl::parser::ast::Node>
    using SharedNode = ccl::parser::ast::SharedNode<T>;

    enum class ControlflowStatus : u32
    {
        NONE,
        RETURN,
        BREAK,
        CONTINUE,
        IF_BLOCK_FINISHES,
        IF_CONDITION_FAILED,
    };

    struct ComputationResult
    {
        Value value;
        ControlflowStatus controlflowStatus{ControlflowStatus::NONE};
    };

    class AstlangNode : public ccl::parser::ast::NonTerminal
    {
    public:
        using NonTerminal::NonTerminal;

        auto runRecursiveOptimization() -> void;

        virtual auto optimize() -> SharedNode<>;

        virtual auto compute(interpreter::Interpreter &interpreter) const -> ComputationResult
        {
            return ComputationResult{.value = Value{}};
        }

        static auto buildConversionTable(const ccl::parser::reader::ParserBuilder &constructor)
            -> ConversionTable;
    };
}// namespace astlang2::ast::core

#endif /* ASTLANG_2_NODE_HPP */
