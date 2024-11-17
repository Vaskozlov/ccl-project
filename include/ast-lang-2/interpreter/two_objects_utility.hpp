#ifndef AST_LANG_2_TWO_OBJECTS_UTILITY_HPP
#define AST_LANG_2_TWO_OBJECTS_UTILITY_HPP

#include <ast-lang-2/ast-lang.hpp>

namespace astlang2::interpreter
{
    template<typename T>
    class NonChangingBinaryOperation
    {
    public:
        template<typename LHS, typename RHS>
        auto operator()(
            ts::Type *return_type, std::shared_ptr<LHS> lhs, std::shared_ptr<RHS> rhs) const -> auto
        {
            return Value{
                .object = std::make_shared<decltype(T{}(*lhs, *rhs))>(T{}(*lhs, *rhs)),
                .type = return_type,
                .valueType = ValueType::R_VALUE,
            };
        }
    };

    template<typename T>
    class ReturningLeftBinaryOperation
    {
    public:
        template<typename LHS, typename RHS>
        auto operator()(
            ts::Type *return_type, std::shared_ptr<LHS> lhs, std::shared_ptr<RHS> rhs) const -> auto
        {
            T{}(*lhs, *rhs);

            return Value{
                .object = lhs,
                .type = return_type,
                .valueType = ValueType::R_VALUE,
            };
        }
    };

    struct Plus : NonChangingBinaryOperation<std::plus<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Minus : NonChangingBinaryOperation<std::minus<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Multiplies : NonChangingBinaryOperation<std::multiplies<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Divides : NonChangingBinaryOperation<std::divides<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Modulus : NonChangingBinaryOperation<std::modulus<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Less : NonChangingBinaryOperation<std::less<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct LessEqual : NonChangingBinaryOperation<std::less_equal<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct Greater : NonChangingBinaryOperation<std::greater<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct GreaterEqual : NonChangingBinaryOperation<std::greater_equal<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct EqualTo : NonChangingBinaryOperation<std::equal_to<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct NotEqualTo : NonChangingBinaryOperation<std::not_equal_to<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct BitAnd : NonChangingBinaryOperation<std::bit_and<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct BitOr : NonChangingBinaryOperation<std::bit_or<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct BitXor : NonChangingBinaryOperation<std::bit_xor<>>
    {
        using NonChangingBinaryOperation::NonChangingBinaryOperation;
    };

    struct DoAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs = std::forward<RHS>(rhs);
        }
    };

    struct DoPlusAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs += std::forward<RHS>(rhs);
        }
    };

    struct DoMinusAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs -= std::forward<RHS>(rhs);
        }
    };

    struct DoMultipliesAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs *= std::forward<RHS>(rhs);
        }
    };

    struct DoDividesAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs /= std::forward<RHS>(rhs);
        }
    };

    struct DoModulusAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs %= std::forward<RHS>(rhs);
        }
    };

    struct DoBitAndAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs &= std::forward<RHS>(rhs);
        }
    };

    struct DoBitOrAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs |= std::forward<RHS>(rhs);
        }
    };

    struct DoBitXorAssign
    {
        template<typename LHS, typename RHS>
        auto operator()(LHS &lhs, RHS &&rhs) -> LHS &
        {
            return lhs ^= std::forward<RHS>(rhs);
        }
    };

    struct Assign : ReturningLeftBinaryOperation<DoAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct PlusAssign : ReturningLeftBinaryOperation<DoPlusAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct MinusAssign : ReturningLeftBinaryOperation<DoMinusAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct MultipliesAssign : ReturningLeftBinaryOperation<DoMultipliesAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct DividesAssign : ReturningLeftBinaryOperation<DoDividesAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct ModulusAssign : ReturningLeftBinaryOperation<DoModulusAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct BitAndAssign : ReturningLeftBinaryOperation<DoBitAndAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct BitOrAssign : ReturningLeftBinaryOperation<DoBitOrAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };

    struct BitXorAssign : ReturningLeftBinaryOperation<DoBitXorAssign>
    {
        using ReturningLeftBinaryOperation::ReturningLeftBinaryOperation;
    };
}// namespace astlang2::interpreter

#endif /* AST_LANG_2_TWO_OBJECTS_UTILITY_HPP */
