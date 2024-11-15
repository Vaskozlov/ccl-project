#ifndef AST_LANG_2_FUNCTIONS_HOLDER_HPP
#define AST_LANG_2_FUNCTIONS_HOLDER_HPP

#include <ast-lang-2/function/function.hpp>

namespace astlang2::function
{
    class FunctionsHolder
    {
    private:
        std::map<FunctionIdentification, std::shared_ptr<Function>> functions;

    public:
        auto addFunction(FunctionIdentification identification, std::shared_ptr<Function> function)
            -> void;

        auto getFunction(const FunctionIdentification &identification) -> Function *;
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_FUNCTIONS_HOLDER_HPP */
