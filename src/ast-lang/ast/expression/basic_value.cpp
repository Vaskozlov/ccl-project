#include <ast-lang/ast/expression/basic_value.hpp>

namespace astlang::ast::expression
{
    auto BasicValue::optimize() -> ccl::parser::ast::Node *
    {
        if (size() != 1) {
            runRecursiveOptimization();
            return nullptr;
        }

        auto *front_ptr = dynamic_cast<Node *>(nodes.front());

        if (front_ptr == nullptr) {
            return nullptr;
        }

        auto optimized_front = front_ptr->optimize();

        if (optimized_front == nullptr) {
            return std::move(nodes.front());
        }

        return optimized_front;
    }
}// namespace astlang::ast::expression
