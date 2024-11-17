#include <ast-lang-2/ast/statement/for_loop.hpp>

namespace astlang2::ast::statement {
    ForLoop::ForLoop(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id} {
    }


    auto ForLoop::compute(interpreter::Interpreter&interpreter) const -> core::ComputationResult {
    }

    auto ForLoop::castChildren(const ConversionTable&conversion_table) -> void {
    }

    auto ForLoop::optimize() -> core::SharedNode<> {
        return nullptr;
    }
} // namespace astlang2::ast::statement
