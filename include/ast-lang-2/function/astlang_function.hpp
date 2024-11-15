#ifndef AST_LANG_2_ASTLANG_FUNCTION_HPP
#define AST_LANG_2_ASTLANG_FUNCTION_HPP

#include <ast-lang-2/ast/core/node.hpp>
#include <ast-lang-2/function/function.hpp>

namespace astlang2::function
{
    class AstlangFunction final : public Function
    {
    private:
        const ast::core::AstlangNode *functionBody{};

    public:
        explicit AstlangFunction(
            std::vector<std::string> arguments_names, const ast::core::AstlangNode *function_body)
          : Function{std::move(arguments_names)}
          , functionBody{function_body}
        {}

    protected:
        auto doCall(interpreter::Interpreter &interpreter) const -> Value override {
            return functionBody->compute(interpreter).value;
        }
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_ASTLANG_FUNCTION_HPP */
