#ifndef AST_LANG_2_ELIF_BLOCK_HPP
#define AST_LANG_2_ELIF_BLOCK_HPP

#include <ast-lang-2/ast/statement/if_block.hpp>

namespace astlang2::ast::statement
{
    class ElifBlock final : public IfBlock
    {
    public:
        using IfBlock::IfBlock;

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;
    };
}// namespace astlang2::ast::statement

#endif /* AST_LANG_2_ELIF_BLOCK_HPP */
