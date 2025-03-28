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

namespace astlang2::ast
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

    class AstlangNode : public ccl::parser::ast::Node
    {
    public:
        using Node::Node;

        using ChildrenNodesGenerator = isl::SmallFunction<ccl::parser::ast::SharedNode<>()>;

        virtual auto optimize() -> SharedNode<> = 0;

        virtual auto compute(interpreter::Interpreter & /*unused*/) const -> ComputationResult
        {
            return ComputationResult{.value = Value{}};
        }

        auto print(
            const std::string &prefix, bool is_left,
            const std::function<std::string(ccl::SmallId)> &id_converter) const -> void override;

        static auto buildConversionTable(const ccl::parser::reader::ParserBuilder &constructor)
            -> ConversionTable;

        template<std::derived_from<Node> T = AstlangNode>
        static auto exchangeIfNotNull(SharedNode<T> &node, SharedNode<> new_node) -> void
        {
            if (new_node != nullptr) {
                node = isl::staticPointerCast<T>(std::move(new_node));
            }
        }
    };
}// namespace astlang2::ast

#endif /* ASTLANG_2_NODE_HPP */
