#include <ast-lang-2/ast/core/basic_value.hpp>

namespace astlang2::ast::core
{
    auto BasicValue::optimize() -> SharedNode<>
    {
        if (size() != 1) {
            runRecursiveOptimization();
            return nullptr;
        }

        auto *front_ptr = dynamic_cast<AstlangNode *>(nodes.front().get());

        if (front_ptr == nullptr) {
            return nullptr;
        }

        auto optimized_front = front_ptr->optimize();

        if (optimized_front == nullptr) {
            return std::move(nodes.front());
        }

        return optimized_front;
    }
}// namespace astlang2::ast::core