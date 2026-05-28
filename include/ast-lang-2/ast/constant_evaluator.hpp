#ifndef AST_LANG_2_CONSTANT_EVALUATOR_HPP
#define AST_LANG_2_CONSTANT_EVALUATOR_HPP

#include <ast-lang-2/ast-lang.hpp>
#include <ast-lang-2/ast/node.hpp>
#include <ast-lang-2/ts/value.hpp>
#include <isl/small_vector.hpp>
#include <optional>
#include <string>

namespace astlang2::ast
{
    namespace expression { class Factor; }

    struct ConstantEvalResult
    {
        NodeTypes resultType;
        AstlangObject<> value;
    };

    struct ExtractedConstant
    {
        NodeTypes type;
        isl::ssize_t intValue{};
        double floatValue{};
        bool boolValue{};
        std::string stringValue{};
    };

    class ConstantEvaluator
    {
    public:
        static auto extractInt(AstlangObject<> obj) -> isl::ssize_t;
        static auto extractDouble(AstlangObject<> obj) -> double;
        static auto extractBool(AstlangObject<> obj) -> bool;
        static auto extractString(AstlangObject<> obj) -> const std::string &;

        static auto evaluateBinary(
            const std::string &opName,
            const ExtractedConstant &lhs, const ExtractedConstant &rhs)
            -> std::optional<ConstantEvalResult>;

        static auto evaluateUnary(
            const std::string &opName,
            const ExtractedConstant &arg) -> std::optional<ConstantEvalResult>;

        static auto evaluateBuiltin(
            const std::string &funcName,
            isl::SmallVector<ExtractedConstant, 4> args)
            -> std::optional<ConstantEvalResult>;

        static auto tryExtractConstant(const SharedNode<> &node)
            -> std::optional<ExtractedConstant>;
    };
}// namespace astlang2::ast

#endif /* AST_LANG_2_CONSTANT_EVALUATOR_HPP */
