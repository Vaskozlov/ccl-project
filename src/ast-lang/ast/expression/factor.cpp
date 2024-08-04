#include <ast-lang/ast/expression/factor.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    static auto constructNumber(NodePtr node) -> EvaluationResult
    {
        const auto *token_node = node.tokenNode;
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        auto value = isl::ssize_t{};
        std::from_chars(repr.data(), repr.data() + repr.size(), value);

        return interpreter::EvaluationResult{isl::UniqueAny(std::move(value)), Type::INT};
    }

    static auto constructString(NodePtr node) -> EvaluationResult
    {
        const auto *token_node = node.tokenNode;
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();
        const auto repr_without_quotes = repr.substr(1, repr.size() - 2);
        auto repr_with_escaping = ccl::text::removeEscaping(repr_without_quotes, {});

        return interpreter::EvaluationResult{
            isl::makeAny<std::string>(repr_with_escaping), Type::STRING};
    }

    static auto readVariable(Interpreter &interpreter, NodePtr node) -> EvaluationResult &
    {
        const auto *token_node = node.tokenNode;
        const auto &token = token_node->getToken();
        const auto repr = token.getRepr();

        return interpreter.read(std::string{repr});
    }

    auto Factor::compute(Interpreter &interpreter) -> EvaluationResult
    {
        if (this->size() != 1) {
            return computeExpression(interpreter);
        }

        auto node = NodePtr{this->front()};
        auto node_type = node.cclNode->getType();

        if (node_type == interpreter.getRuleId("NUMBER")) {
            return constructNumber(NodePtr{node});
        }

        if (node_type == interpreter.getRuleId("STRING")) {
            return constructString(NodePtr{node});
        }

        if (node_type == interpreter.getRuleId("IDENTIFIER")) {
            auto &read_result = readVariable(interpreter, NodePtr{node});

            return EvaluationResult{
                .value = isl::makeAny<EvaluationResult *>(&read_result),
                .type = read_result.type,
                .storesReference = true,
            };
        }

        if (node_type == interpreter.getRuleId("FUNCTION_CALL")) {
            return node.astlangNode->compute(interpreter);
        }

        if (node_type == interpreter.getRuleId("METHOD_CALL")) {
            return node.astlangNode->compute(interpreter);
        }

        throw std::runtime_error("Unknown factor type");
    }

    auto Factor::computeExpression(Interpreter &interpreter) -> EvaluationResult
    {
        auto middle_node = NodePtr{this->at(1)};
        return middle_node.astlangNode->compute(interpreter);
    }
}// namespace astlang::ast::expression