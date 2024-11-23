#ifndef AST_LANG_2_ASTLANG_FUNCTION_HPP
#define AST_LANG_2_ASTLANG_FUNCTION_HPP

#include <ast-lang-2/ast/node.hpp>
#include <ast-lang-2/function/function.hpp>

namespace astlang2::function
{
    class AstlangFunction final : public Function
    {
    private:
        ast::SharedNode<ast::AstlangNode> functionBody;

    public:
        explicit AstlangFunction(
            const isl::SmallVector<std::string, 2> &arguments_names,
            ast::SharedNode<ast::AstlangNode>
                function_body)
          : Function{arguments_names}
          , functionBody{std::move(function_body)}
        {}

    protected:
        auto doCall(interpreter::Interpreter &interpreter) const -> Value override
        {
            return functionBody->compute(interpreter).value;
        }
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_ASTLANG_FUNCTION_HPP */
