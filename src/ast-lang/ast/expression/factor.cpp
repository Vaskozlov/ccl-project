#include <ast-lang/ast/expression/factor.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <charconv>

namespace astlang::ast::expression
{
    using namespace interpreter;

    static auto constructNumber(Interpreter &interpreter, const ConstNodePtr node)
        -> EvaluationResult
    {
        const auto &token = node.terminalNode->getToken();
        const auto repr = token.getRepr();

        auto value = isl::ssize_t{};
        std::from_chars(repr.data(), repr.data() + repr.size(), value);

        auto instance = Instance{.type = interpreter.typeSystem.getType("int")};
        instance.fields.emplace("value", isl::makeAny<isl::ssize_t>(value));

        return EvaluationResult{
            .instance = std::move(instance),
            .needToReturn = false,
            .storesReference = false,
        };
    }

    static auto constructString(Interpreter &interpreter, const ConstNodePtr node)
        -> EvaluationResult
    {
        const auto &token = node.terminalNode->getToken();
        const auto repr = token.getRepr();
        const auto repr_without_quotes = repr.substr(1, repr.size() - 2);
        auto repr_with_escaping = ccl::text::removeEscaping(repr_without_quotes, {});

        auto instance = Instance{.type = interpreter.typeSystem.getType("string")};
        instance.fields.emplace("value", isl::makeAny<std::string>(repr_with_escaping));

        return EvaluationResult{
            .instance = std::move(instance),
            .needToReturn = false,
            .storesReference = false,
        };
    }

    static auto readVariable(Interpreter &interpreter, const ConstNodePtr node)
        -> EvaluationResult &
    {
        const auto &token = node.terminalNode->getToken();
        return interpreter.read(std::string{token.getRepr()});
    }

    static auto constructBoolean(const Interpreter &interpreter, const ConstNodePtr node)
        -> EvaluationResult
    {
        const auto &token = node.terminalNode->getToken();

        auto instance = Instance{.type = interpreter.typeSystem.getType("bool")};
        instance.fields.emplace("value", isl::makeAny<bool>(token.getId() == interpreter.TRUE));

        return EvaluationResult{
            .instance = std::move(instance),
            .needToReturn = false,
            .storesReference = false,
        };
    }

    auto Factor::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        if (this->size() != 1) {
            return computeExpression(interpreter);
        }

        const auto node = ConstNodePtr{front().get()};
        const auto node_type = node.cclNode->getType();

        if (node_type == interpreter.NUMBER) {
            // return constructNumber(node);
        }

        if (node_type == interpreter.TRUE || node_type == interpreter.FALSE) {
            return constructBoolean(interpreter, ConstNodePtr{node});
        }

        if (node_type == interpreter.STRING) {
            // return constructString(ConstNodePtr{node});
        }

        if (node_type == interpreter.IDENTIFIER) {
            auto &read_result = readVariable(interpreter, ConstNodePtr{node});

            return EvaluationResult{
                // .value = isl::makeAny<EvaluationResult *>(&read_result),
                // .type = read_result.type,
                // .storesReference = true,
            };
        }

        if (node_type == interpreter.FUNCTION_CALL) {
            return node.astlangNode->compute(interpreter);
        }

        if (node_type == interpreter.METHOD_CALL) {
            return node.astlangNode->compute(interpreter);
        }

        throw std::runtime_error("Unknown factor type");
    }

    auto Factor::computeExpression(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto middle_node = ConstNodePtr{at(1).get()};
        return middle_node.astlangNode->compute(interpreter);
    }
}// namespace astlang::ast::expression