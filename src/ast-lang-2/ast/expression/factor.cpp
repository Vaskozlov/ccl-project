#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/text/remove_escaping.hpp>
#include <charconv>

namespace astlang2::ast::expression
{
    static auto constructNumberNode(
        const interpreter::Interpreter &interpreter, const ccl::parser::ast::Node *node)
        -> astlang2::Value
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        auto parsed_number = isl::ssize_t{};
        std::from_chars(repr.begin(), repr.end(), parsed_number);

        return astlang2::Value{
            .object = std::make_shared<isl::ssize_t>(parsed_number),
            .type = interpreter.getInt(),
        };
    }

    static auto constructFloatNode(
        const interpreter::Interpreter &interpreter, const ccl::parser::ast::Node *node)
        -> astlang2::Value
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        auto parsed_number = std::stod(static_cast<std::string>(repr));

        return astlang2::Value{
            .object = std::make_shared<double>(parsed_number),
            .type = interpreter.getDouble(),
        };
    }

    static auto constructStringNode(
        const interpreter::Interpreter &interpreter, const ccl::parser::ast::Node *node)
        -> astlang2::Value
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        const auto repr_without_quotes = repr.substr(1).withoutLastSymbol();
        const auto repr_with_processed_escapes = ccl::text::removeEscaping(repr_without_quotes, {});

        return astlang2::Value{
            .object = std::make_shared<std::string>(repr_with_processed_escapes),
            .type = interpreter.getString(),
        };
    }

    static auto constructBooleanNode(
        const interpreter::Interpreter &interpreter, const ccl::parser::ast::Node *node)
        -> astlang2::Value
    {
        return astlang2::Value{
            .object = std::make_shared<bool>(node->getType() == interpreter.TRUE),
            .type = interpreter.getBool(),
        };
    }

    static auto
        readVariable(interpreter::Interpreter &interpreter, const ccl::parser::ast::Node *node)
            -> astlang2::Value
    {
        const auto *token_node = static_cast<const ccl::parser::ast::Terminal *>(node);
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        return interpreter.read(static_cast<std::string>(repr));
    }

    auto Factor::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        using namespace core;

        if (size() != 1) {
            return static_cast<AstlangNode *>(at(1).get())->compute(interpreter);
        }

        const auto *front_node = static_cast<const Node *>(front().get());
        const auto node_type = front_node->getType();

        if (node_type == interpreter.NUMBER) {
            return ComputationResult{.value = constructNumberNode(interpreter, front_node)};
        }

        if (node_type == interpreter.FLOAT) {
            return ComputationResult{.value = constructFloatNode(interpreter, front_node)};
        }

        if (node_type == interpreter.TRUE || node_type == interpreter.FALSE) {
            return ComputationResult{.value = constructBooleanNode(interpreter, front_node)};
        }

        if (node_type == interpreter.STRING) {
            return ComputationResult{.value = constructStringNode(interpreter, front_node)};
        }

        if (node_type == interpreter.IDENTIFIER) {
            return ComputationResult{.value = readVariable(interpreter, front_node)};
        }

        if (node_type == interpreter.FUNCTION_CALL || node_type == interpreter.METHOD_CALL) {
            return static_cast<const AstlangNode *>(front_node)->compute(interpreter);
        }

        throw std::runtime_error{"Unknown factor type"};
    }
}// namespace astlang2::ast::expression