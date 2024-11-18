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
        -> core::ComputationResult
    {
        core::ComputationResult object_value = objectNode->compute(interpreter);

        if (object_value.controlflowStatus == core::ControlflowStatus::RETURN) {
            return object_value;
        }

        auto *object_map = static_cast<Instance *>(object_value.value.object.get());
        return core::ComputationResult{.value = object_map->at(fieldName)};
    }

    auto FieldAccess::castChildren(const ConversionTable &conversion_table) -> void
    {
        objectNode->cast(conversion_table);
    }

    auto FieldAccess::optimize() -> core::SharedNode<>
    {
        exchangeIfNotNull(objectNode, objectNode->optimize());
        return nullptr;
    }
}// namespace astlang2::ast::expression