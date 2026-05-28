#include <ast-lang-2/ast/constant_evaluator.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <cmath>
#include <functional>

namespace astlang2::ast
{
    auto ConstantEvaluator::extractInt(AstlangObject<> obj) -> isl::ssize_t
    {
        return *isl::staticPointerCast<isl::ssize_t>(std::move(obj));
    }

    auto ConstantEvaluator::extractDouble(AstlangObject<> obj) -> double
    {
        return *isl::staticPointerCast<double>(std::move(obj));
    }

    auto ConstantEvaluator::extractBool(AstlangObject<> obj) -> bool
    {
        return *isl::staticPointerCast<bool>(std::move(obj));
    }

    auto ConstantEvaluator::extractString(AstlangObject<> obj) -> const std::string &
    {
        return *isl::staticPointerCast<std::string>(std::move(obj));
    }

    namespace
    {
        template<typename T>
        auto makeResult(NodeTypes type, T &&val) -> ConstantEvalResult
        {
            return {type, AstlangObject<std::decay_t<T>>(std::forward<T>(val))};
        }

        inline auto makeBoolResult(bool v) -> ConstantEvalResult
        {
            return {v ? NodeTypes::TRUE_VALUE : NodeTypes::FALSE_VALUE,
                    AstlangObject<bool>(v)};
        }

        template<typename T>
        auto safeDivide(T lhs, T rhs) -> std::optional<T>
        {
            if (rhs == T{}) return std::nullopt;
            return lhs / rhs;
        }

        template<typename T>
        auto safeModulus(T lhs, T rhs) -> std::optional<T>
        {
            if (rhs == T{}) return std::nullopt;
            return lhs % rhs;
        }

        // Evaluate a binary arithmetic/comparison operation on two typed values of the same NodeTypes
        template<typename T, typename Op>
        auto evalArithmetic(T lhs, T rhs, Op op, NodeTypes type) -> std::optional<ConstantEvalResult>
        {
            return makeResult(type, op(lhs, rhs));
        }

        template<typename T, typename Op>
        auto evalComparison(T lhs, T rhs, Op op) -> std::optional<ConstantEvalResult>
        {
            return makeBoolResult(op(lhs, rhs));
        }

        auto evalNumberBinary(
            const std::string &opName,
            isl::ssize_t lhs, isl::ssize_t rhs) -> std::optional<ConstantEvalResult>
        {
            if (opName == "__add__") return makeResult(NodeTypes::NUMBER, lhs + rhs);
            if (opName == "__sub__") return makeResult(NodeTypes::NUMBER, lhs - rhs);
            if (opName == "__mul__") return makeResult(NodeTypes::NUMBER, lhs * rhs);
            if (opName == "__div__") {
                auto r = safeDivide(lhs, rhs);
                if (!r) return std::nullopt;
                return makeResult(NodeTypes::NUMBER, *r);
            }
            if (opName == "__mod__") {
                auto r = safeModulus(lhs, rhs);
                if (!r) return std::nullopt;
                return makeResult(NodeTypes::NUMBER, *r);
            }
            if (opName == "__less__") return makeBoolResult(lhs < rhs);
            if (opName == "__less_eq__") return makeBoolResult(lhs <= rhs);
            if (opName == "__greater__") return makeBoolResult(lhs > rhs);
            if (opName == "__greater_eq__") return makeBoolResult(lhs >= rhs);
            if (opName == "__equal__") return makeBoolResult(lhs == rhs);
            if (opName == "__not_equal__") return makeBoolResult(lhs != rhs);
            if (opName == "__bitwise_and__") return makeResult(NodeTypes::NUMBER, lhs & rhs);
            if (opName == "__bitwise_or__") return makeResult(NodeTypes::NUMBER, lhs | rhs);
            if (opName == "__bitwise_xor__") return makeResult(NodeTypes::NUMBER, lhs ^ rhs);
            return std::nullopt;
        }

        auto evalFloatBinary(
            const std::string &opName,
            double lhs, double rhs) -> std::optional<ConstantEvalResult>
        {
            if (opName == "__add__") return makeResult(NodeTypes::FLOAT, lhs + rhs);
            if (opName == "__sub__") return makeResult(NodeTypes::FLOAT, lhs - rhs);
            if (opName == "__mul__") return makeResult(NodeTypes::FLOAT, lhs * rhs);
            if (opName == "__div__") {
                if (rhs == 0.0) return std::nullopt;
                return makeResult(NodeTypes::FLOAT, lhs / rhs);
            }
            if (opName == "__mod__") {
                if (rhs == 0.0) return std::nullopt;
                return makeResult(NodeTypes::FLOAT, std::fmod(lhs, rhs));
            }
            if (opName == "__less__") return makeBoolResult(lhs < rhs);
            if (opName == "__less_eq__") return makeBoolResult(lhs <= rhs);
            if (opName == "__greater__") return makeBoolResult(lhs > rhs);
            if (opName == "__greater_eq__") return makeBoolResult(lhs >= rhs);
            if (opName == "__equal__") return makeBoolResult(lhs == rhs);
            if (opName == "__not_equal__") return makeBoolResult(lhs != rhs);
            return std::nullopt;
        }

        auto evalStringBinary(
            const std::string &opName,
            const std::string &lhs, const std::string &rhs) -> std::optional<ConstantEvalResult>
        {
            if (opName == "__add__") return makeResult(NodeTypes::STRING, lhs + rhs);
            if (opName == "__less__") return makeBoolResult(lhs < rhs);
            if (opName == "__less_eq__") return makeBoolResult(lhs <= rhs);
            if (opName == "__greater__") return makeBoolResult(lhs > rhs);
            if (opName == "__greater_eq__") return makeBoolResult(lhs >= rhs);
            if (opName == "__equal__") return makeBoolResult(lhs == rhs);
            if (opName == "__not_equal__") return makeBoolResult(lhs != rhs);
            return std::nullopt;
        }

        auto evalBoolBinary(
            const std::string &opName,
            bool lhs, bool rhs) -> std::optional<ConstantEvalResult>
        {
            if (opName == "__logical_and__") return makeBoolResult(lhs && rhs);
            if (opName == "__logical_or__") return makeBoolResult(lhs || rhs);
            if (opName == "__equal__") return makeBoolResult(lhs == rhs);
            if (opName == "__not_equal__") return makeBoolResult(lhs != rhs);
            return std::nullopt;
        }
    } // namespace

    auto ConstantEvaluator::evaluateBinary(
        const std::string &opName,
        const ExtractedConstant &lhs, const ExtractedConstant &rhs) -> std::optional<ConstantEvalResult>
    {
        bool is_bool = (lhs.type == NodeTypes::TRUE_VALUE || lhs.type == NodeTypes::FALSE_VALUE);
        bool rhs_is_bool = (rhs.type == NodeTypes::TRUE_VALUE || rhs.type == NodeTypes::FALSE_VALUE);

        if (!is_bool && !rhs_is_bool && lhs.type != rhs.type) {
            return std::nullopt;
        }

        if (is_bool != rhs_is_bool) {
            return std::nullopt;
        }

        if (opName == "__assign__") {
            return std::nullopt;
        }

        switch (lhs.type) {
        case NodeTypes::NUMBER:
            return evalNumberBinary(opName, lhs.intValue, rhs.intValue);
        case NodeTypes::FLOAT:
            return evalFloatBinary(opName, lhs.floatValue, rhs.floatValue);
        case NodeTypes::STRING:
            return evalStringBinary(opName, lhs.stringValue, rhs.stringValue);
        case NodeTypes::TRUE_VALUE:
        case NodeTypes::FALSE_VALUE:
            return evalBoolBinary(opName, lhs.boolValue, rhs.boolValue);
        default:
            return std::nullopt;
        }
    }

    auto ConstantEvaluator::evaluateUnary(
        const std::string &opName,
        const ExtractedConstant &arg) -> std::optional<ConstantEvalResult>
    {
        if (opName == "__negation__") {
            if (arg.type == NodeTypes::NUMBER) {
                return makeResult(NodeTypes::NUMBER, -arg.intValue);
            }
            if (arg.type == NodeTypes::FLOAT) {
                return makeResult(NodeTypes::FLOAT, -arg.floatValue);
            }
        }

        if (opName == "__positive__") {
            if (arg.type == NodeTypes::NUMBER) {
                return makeResult(NodeTypes::NUMBER, +arg.intValue);
            }
            if (arg.type == NodeTypes::FLOAT) {
                return makeResult(NodeTypes::FLOAT, +arg.floatValue);
            }
        }

        if (opName == "__not__") {
            if (arg.type == NodeTypes::TRUE_VALUE || arg.type == NodeTypes::FALSE_VALUE) {
                return makeBoolResult(!arg.boolValue);
            }
        }

        if (opName == "__bitwise_not__") {
            if (arg.type == NodeTypes::NUMBER) {
                return makeResult(NodeTypes::NUMBER, ~arg.intValue);
            }
        }

        return std::nullopt;
    }

    auto ConstantEvaluator::evaluateBuiltin(
        const std::string &funcName,
        isl::SmallVector<ExtractedConstant, 4> args)
        -> std::optional<ConstantEvalResult>
    {
        if (args.empty()) {
            return std::nullopt;
        }

        auto &arg = args[0];
        NodeTypes argType = arg.type;

        if (funcName == "int") {
            isl::ssize_t val{};
            if (argType == NodeTypes::NUMBER) {
                val = arg.intValue;
            } else if (argType == NodeTypes::FLOAT) {
                val = static_cast<isl::ssize_t>(arg.floatValue);
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue ? 1 : 0;
            } else {
                return std::nullopt;
            }
            return makeResult(NodeTypes::NUMBER, val);
        }

        if (funcName == "uint") {
            std::size_t val{};
            if (argType == NodeTypes::NUMBER) {
                if (arg.intValue < 0) val = 0;
                else val = static_cast<std::size_t>(arg.intValue);
            } else if (argType == NodeTypes::FLOAT) {
                if (arg.floatValue < 0.0) val = 0;
                else val = static_cast<std::size_t>(arg.floatValue);
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue ? 1 : 0;
            } else {
                return std::nullopt;
            }
            return makeResult(NodeTypes::NUMBER, val);
        }

        if (funcName == "float") {
            double val{};
            if (argType == NodeTypes::NUMBER) {
                val = static_cast<double>(arg.intValue);
            } else if (argType == NodeTypes::FLOAT) {
                val = arg.floatValue;
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue ? 1.0 : 0.0;
            } else {
                return std::nullopt;
            }
            return makeResult(NodeTypes::FLOAT, val);
        }

        if (funcName == "double") {
            double val{};
            if (argType == NodeTypes::NUMBER) {
                val = static_cast<double>(arg.intValue);
            } else if (argType == NodeTypes::FLOAT) {
                val = arg.floatValue;
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue ? 1.0 : 0.0;
            } else {
                return std::nullopt;
            }
            return makeResult(NodeTypes::FLOAT, val);
        }

        if (funcName == "bool") {
            bool val{};
            if (argType == NodeTypes::NUMBER) {
                val = arg.intValue != 0;
            } else if (argType == NodeTypes::FLOAT) {
                val = arg.floatValue != 0.0;
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue;
            } else if (argType == NodeTypes::STRING) {
                val = !arg.stringValue.empty();
            } else {
                return std::nullopt;
            }
            return makeBoolResult(val);
        }

        if (funcName == "string") {
            std::string val;
            if (argType == NodeTypes::NUMBER) {
                val = fmt::to_string(arg.intValue);
            } else if (argType == NodeTypes::FLOAT) {
                val = fmt::to_string(arg.floatValue);
            } else if (argType == NodeTypes::TRUE_VALUE || argType == NodeTypes::FALSE_VALUE) {
                val = arg.boolValue ? "true" : "false";
            } else if (argType == NodeTypes::STRING) {
                val = arg.stringValue;
            } else {
                return std::nullopt;
            }
            return makeResult(NodeTypes::STRING, std::move(val));
        }

        return std::nullopt;
    }

    auto ConstantEvaluator::tryExtractConstant(const SharedNode<> &node)
        -> std::optional<ExtractedConstant>
    {
        if (node == nullptr) {
            return std::nullopt;
        }

        auto *factor = dynamic_cast<expression::Factor *>(node.get());
        if (!factor) {
            return std::nullopt;
        }

        const auto &value = factor->getPrecomputedValue();
        if (value == nullptr) {
            return std::nullopt;
        }

        auto type = factor->getNodeType();
        ExtractedConstant result{};
        result.type = type;

        switch (type) {
        case NodeTypes::NUMBER:
            result.intValue = *isl::staticPointerCast<isl::ssize_t>(AstlangObject<>(value));
            return result;
        case NodeTypes::FLOAT:
            result.floatValue = *isl::staticPointerCast<double>(AstlangObject<>(value));
            return result;
        case NodeTypes::STRING:
            result.stringValue = *isl::staticPointerCast<std::string>(AstlangObject<>(value));
            return result;
        case NodeTypes::TRUE_VALUE:
        case NodeTypes::FALSE_VALUE:
            result.boolValue = *isl::staticPointerCast<bool>(AstlangObject<>(value));
            return result;
        default:
            return std::nullopt;
        }
    }

}// namespace astlang2::ast
