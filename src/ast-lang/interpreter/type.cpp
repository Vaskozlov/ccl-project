#include <ast-lang/interpreter/type.hpp>

namespace astlang::interpreter
{
    auto Type::fromNode(const ccl::parser::ast::Node *node) -> Type
    {
        const auto node_ptr = ast::ConstNodePtr{node};
        const auto front_node = ast::ConstNodePtr{node_ptr.nonTerminalNode->front().get()};
        auto front_token = front_node.terminalNode->getToken();
        auto main_type = StringToCoreType.at(front_token.getRepr());

        auto sub_types = std::vector<Type>{};

        if (node_ptr.nonTerminalNode->size() > 1) {
            sub_types.emplace_back(Type::fromNode(node_ptr.nonTerminalNode->at(1).get()));
        }

        return Type{main_type, sub_types};
    }

    auto Type::operator==(const Type &other) const -> bool
    {
        return mainType == other.mainType && subTypes == other.subTypes;
    }

    auto Type::operator<=>(const Type &other) const -> std::weak_ordering
    {
        if (mainType != other.mainType) {
            return mainType <=> other.mainType;
        }

        return subTypes <=> other.subTypes;
    }

    auto Type::toString() const -> std::string
    {
        auto main_type_repr = CoreTypeToString.at(mainType);

        if (isTemplate()) {
            auto range_of_subtypes = std::views::transform(subTypes, [](auto &type) {
                return type.toString();
            });

            return fmt::format("{}[{}]", main_type_repr, range_of_subtypes);
        }

        return std::string{main_type_repr};
    }
}// namespace astlang::interpreter
