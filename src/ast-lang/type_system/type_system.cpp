#include <ast-lang/type_system/type_system.hpp>

namespace astlang::type_system
{
    auto TypeSystem::getType(const ccl::parser::ast::Node *node) const -> Type *
    {
        const auto *non_terminal_ptr = ast::ConstNodePtr{node}.nonTerminalNode;
        // const auto *front_token = ast::ConstNodePtr{non_terminal_ptr->front()}.terminalNode;
        // const std::string type_name = front_token->getToken().getRepr();

        // auto it = typeMap.find(type_name);

        // if (it == typeMap.end()) {
            // return nullptr;
        // }

        // return it->second.get();
    }
}// namespace astlang::type_system