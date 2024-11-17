#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/text/remove_escaping.hpp>
#include <charconv>

namespace astlang2::ast::expression
{
    static auto constructNumberNode(const ccl::parser::ast::Node *node) -> std::shared_ptr<void>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        auto parsed_number = isl::ssize_t{};
        std::from_chars(repr.begin(), repr.end(), parsed_number);

        return std::make_shared<isl::ssize_t>(parsed_number);
    }

    static auto constructFloatNode(const ccl::parser::ast::Node *node) -> std::shared_ptr<void>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        auto parsed_number = std::stod(static_cast<std::string>(repr));

        return std::make_shared<double>(parsed_number);
    }

    static auto constructStringNode(const ccl::parser::ast::Node *node) -> std::shared_ptr<void>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        const auto repr_without_quotes = repr.substr(1).withoutLastSymbol();
        const auto repr_with_processed_escapes = ccl::text::removeEscaping(repr_without_quotes, {});

        return std::make_shared<std::string>(repr_with_processed_escapes);
    }

    static auto constructBooleanNode(bool value) -> std::shared_ptr<void>
    {
        return std::make_shared<bool>(value);
    }

    static auto getVariableName(const ccl::parser::ast::Node *node) -> std::shared_ptr<void>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        return std::make_shared<std::string>(repr);
    }

    Factor::Factor(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
    {
        if (initial_nodes.size() != 1) {
            node = initial_nodes.at(1);
            nodeType = NodeTypes::ASSIGNMENT_EXPRESSION;
            return;
        }

        node = initial_nodes.front();
        nodeType = static_cast<NodeTypes>(node->getType());

        switch (nodeType) {
            using enum NodeTypes;

        case NUMBER:
            precomputedValue = constructNumberNode(node.get());
            break;

        case FLOAT:
            precomputedValue = constructFloatNode(node.get());
            break;

        case TRUE:
            precomputedValue = constructBooleanNode(true);
            break;

        case FALSE:
            precomputedValue = constructBooleanNode(false);
            break;

        case STRING:
            precomputedValue = constructStringNode(node.get());
            break;

        case IDENTIFIER:
            precomputedValue = getVariableName(node.get());
            break;

        default:
            break;
        }
    }

    auto Factor::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        using namespace core;

        switch (nodeType) {
            using enum NodeTypes;

        case NUMBER:
            return ComputationResult{
                .value =
                    astlang2::Value{
                        .object = precomputedValue,
                        .type = interpreter.getInt(),
                    },
            };

        case FLOAT:
            return ComputationResult{
                .value =
                    astlang2::Value{
                        .object = precomputedValue,
                        .type = interpreter.getDouble(),
                    },
            };

        case TRUE:
        case FALSE:
            return ComputationResult{
                .value =
                    astlang2::Value{
                        .object = precomputedValue,
                        .type = interpreter.getBool(),
                    },
            };

        case STRING:
            return ComputationResult{
                .value =
                    astlang2::Value{
                        .object = precomputedValue,
                        .type = interpreter.getString(),
                    },
            };

        case IDENTIFIER:
            return ComputationResult{
                .value = interpreter.read(*static_cast<std::string *>(precomputedValue.get())),
            };

        default:
            return static_cast<AstlangNode *>(node.get())->compute(interpreter);
        }
    }

    auto Factor::castChildren(const ConversionTable &conversion_table) -> void
    {
        node->cast(conversion_table);
    }

    auto Factor::optimize() -> core::SharedNode<>
    {
        if (nodeType != NodeTypes::ASSIGNMENT_EXPRESSION) {
            return nullptr;
        }

        auto *node_ptr = static_cast<AstlangNode *>(node.get());
        auto new_node = node_ptr->optimize();

        if (new_node != nullptr) {
            node = isl::staticPointerCast<Node>(new_node);
        }

        return node;
    }
}// namespace astlang2::ast::expression