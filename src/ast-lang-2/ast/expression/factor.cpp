#include <ast-lang-2/ast-lang.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/text/remove_escaping.hpp>
#include <cassert>
#include <charconv>
#include <vector>

namespace astlang2::ast::expression
{
    static void collectArrayElements(
        const ccl::parser::ast::NonTerminal *arrayElements,
        std::vector<ccl::parser::ast::SharedNode<>> &out)
    {
        if (arrayElements->size() == 1) {
            out.push_back(arrayElements->front());
        } else {
            out.push_back(arrayElements->front());
            const auto *tail = static_cast<const ccl::parser::ast::NonTerminal *>(
                arrayElements->back().get());
            collectArrayElements(tail, out);
        }
    }

    static auto constructNumberNode(const ccl::parser::ast::Node *node) -> AstlangObject<>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        auto parsed_number = isl::ssize_t{};
        std::from_chars(repr.begin(), repr.end(), parsed_number);

        return AstlangObject<isl::ssize_t>(parsed_number);
    }

    static auto constructFloatNode(const ccl::parser::ast::Node *node) -> AstlangObject<>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        auto parsed_number = std::stod(static_cast<std::string>(repr));

        return AstlangObject<double>(parsed_number);
    }

    static auto constructStringNode(const ccl::parser::ast::Node *node) -> AstlangObject<>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        const auto repr_without_quotes = repr.substr(1).withoutLastSymbol();
        const auto repr_with_processed_escapes = ccl::text::removeEscaping(repr_without_quotes, {});

        return AstlangObject<std::string>(repr_with_processed_escapes);
    }

    static auto constructBooleanNode(bool value) -> AstlangObject<>
    {
        return AstlangObject<bool>(value);
    }

    static auto getVariableName(const ccl::parser::ast::Node *node) -> AstlangObject<>
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        return AstlangObject<std::string>(repr);
    }

    Factor::Factor(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
    {
        if (initial_nodes.empty()) {
            // Factory-created constant — members set by caller
            return;
        }

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

        case TRUE_VALUE:
            precomputedValue = constructBooleanNode(true);
            break;

        case FALSE_VALUE:
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

    auto Factor::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
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

        case TRUE_VALUE:
        case FALSE_VALUE:
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

        case ARRAY: {
            const auto *arrayNode =
                static_cast<const ccl::parser::ast::NonTerminal *>(node.get());
            const auto *arrayElements =
                static_cast<const ccl::parser::ast::NonTerminal *>(arrayNode->at(1).get());
            std::vector<ccl::parser::ast::SharedNode<>> rawElements;
            collectArrayElements(arrayElements, rawElements);
            std::vector<astlang2::Value> values;
            for (const auto &rawElem : rawElements) {
                auto typedElem = isl::staticPointerCast<AstlangNode>(rawElem);
                auto result = typedElem->compute(interpreter);
                values.push_back(std::move(result.value));
            }
            return ComputationResult{
                .value = astlang2::Value{
                    .object = AstlangObject<std::vector<astlang2::Value>>(std::move(values)),
                    .type = interpreter.getArray(),
                },
            };
        }

        default:
            return static_cast<AstlangNode *>(node.get())->compute(interpreter);
        }
    }

    auto Factor::optimize() -> SharedNode<>
    {
        if (isConstant || node == nullptr) {
            // Constant-folded or factory-created constant node — nothing to optimize
            return nullptr;
        }

        switch (nodeType) {
            using enum NodeTypes;

        case NUMBER:
        case STRING:
        case FLOAT:
        case IDENTIFIER:
        case TRUE_VALUE:
        case FALSE_VALUE:
        case FIELD_ACCESS:
        case ARRAY:
            return nullptr;

        case ASSIGNMENT_EXPRESSION:
        case FUNCTION_CALL:
        case METHOD_CALL:
            exchangeIfNotNull<Node>(node, static_cast<AstlangNode *>(node.get())->optimize());
            return node;

        default:
            isl::unreachable();
        }
    }

    auto Factor::print(
        const std::string &prefix, const bool is_left,
        const std::function<std::string(SmallId)> &id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
                         "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left),
                         id_converter(getType()))
                  << std::endl;

        if (isConstant) {
            // Print the folded constant value directly
            std::string value_str;
            switch (nodeType) {
            case NodeTypes::NUMBER:
                value_str = fmt::to_string(*isl::staticPointerCast<isl::ssize_t>(AstlangObject<>(precomputedValue)));
                break;
            case NodeTypes::FLOAT:
                value_str = fmt::to_string(*isl::staticPointerCast<double>(AstlangObject<>(precomputedValue)));
                break;
            case NodeTypes::STRING:
                value_str = "\"" + *isl::staticPointerCast<std::string>(AstlangObject<>(precomputedValue)) + "\"";
                break;
            case NodeTypes::TRUE_VALUE:
                value_str = "true";
                break;
            case NodeTypes::FALSE_VALUE:
                value_str = "false";
                break;
            default:
                // Fallback: print child node
                if (node != nullptr) {
                    node->print(prefix, is_left, id_converter);
                    return;
                }
                return;
            }
            std::cout << fmt::format("{}-CONSTANT: {}", getPrintingPrefix(expanded_prefix, false), value_str) << std::endl;
            return;
        }

        // Delegate to child node for non-constant factors
        auto generator = getChildrenNodes();
        SharedNode<> next = generator();

        while (next != nullptr) {
            SharedNode<> child_node = std::move(next);
            next = generator();

            child_node->print(expanded_prefix, next != nullptr, id_converter);
        }
    }

    auto Factor::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [index = 0, stored_node = node, is_const = isConstant]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0 || is_const || stored_node == nullptr) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }

    auto Factor::createConstant(NodeTypes type, AstlangObject<> value) -> SharedNode<>
    {
        assert(AstlangNode::parserBuilder != nullptr);
        const SmallId factorRuleId = AstlangNode::parserBuilder->getRuleId("FACTOR");
        auto typed = SharedNode<Factor>(factorRuleId, ccl::parser::ast::SmallVectorOfNodes{});
        typed->nodeType = type;
        typed->precomputedValue = std::move(value);
        typed->isConstant = true;
        return typed;
    }

}// namespace astlang2::ast::expression