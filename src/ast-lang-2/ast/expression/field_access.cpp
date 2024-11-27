#include <ast-lang-2/ast/expression/field_access.hpp>

namespace astlang2::ast::expression
{
    FieldAccess::FieldAccess(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , objectNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(0))}
    {
        const ccl::lexer::Token &field_name_token =
            static_cast<const ccl::parser::ast::Terminal *>(initial_nodes.at(2).get())->getToken();

        auto field_name_repr = field_name_token.getRepr();

        fieldName = static_cast<std::string>(field_name_repr);
    }

    auto FieldAccess::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        ComputationResult object_value = objectNode->compute(interpreter);

        if (object_value.controlflowStatus == ControlflowStatus::RETURN) {
            return object_value;
        }

        auto *object_map = static_cast<Instance *>(object_value.value.object.get());
        return ComputationResult{.value = object_map->at(fieldName)};
    }

    auto FieldAccess::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(objectNode, objectNode->optimize());
        return nullptr;
    }

    auto FieldAccess::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [index = 0, stored_node = objectNode]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }
}// namespace astlang2::ast::expression