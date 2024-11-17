#include <ast-lang-2/ast/statement/elif_block.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement {
    static constexpr std::size_t ELIF_BLOCK_SIZE_WITHOUT_NEXT_ELIF = 5;

    ElifBlock::ElifBlock(
        const SmallId id,
        const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id} {
        core::SharedNode<ccl::parser::ast::NonTerminal> next_block;

        elifStatements.emplace_back(
            isl::staticPointerCast<AstlangNode>(initial_nodes.at(1)),
            isl::staticPointerCast<AstlangNode>(initial_nodes.at(3)));

        if (initial_nodes.size() == ELIF_BLOCK_SIZE_WITHOUT_NEXT_ELIF) {
            return;
        }

        next_block = isl::staticPointerCast<ccl::parser::ast::NonTerminal>(initial_nodes.back());

        while (true) {
            elifStatements.emplace_back(
                isl::staticPointerCast<AstlangNode>(next_block->at(1)),
                isl::staticPointerCast<AstlangNode>(next_block->at(3)));

            if (next_block->size() == ELIF_BLOCK_SIZE_WITHOUT_NEXT_ELIF) {
                break;
            }

            next_block = isl::staticPointerCast<ccl::parser::ast::NonTerminal>(next_block->back());
        }
    }

    auto ElifBlock::compute(interpreter::Interpreter&interpreter) const -> core::ComputationResult {
        for (const auto&elif_statement: elifStatements) {
            auto condition_result = elif_statement.conditionNode->compute(interpreter);

            if (condition_result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return condition_result;
            }

            const auto condition_to_bool =
                    interpreter.callFunction("bool", {std::move(condition_result.value)});

            if (*static_cast<bool *>(condition_to_bool.object.get())) {
                core::ComputationResult statement_result =
                        elif_statement.bodyNode->compute(interpreter);

                statement_result.controlflowStatus =
                        statement_result.controlflowStatus == core::ControlflowStatus::RETURN
                            ? statement_result.controlflowStatus
                            : core::ControlflowStatus::IF_BLOCK_FINISHES;

                return statement_result;
            }
        }

        return core::ComputationResult{
            .controlflowStatus = core::ControlflowStatus::IF_CONDITION_FAILED,
        };
    }

    auto ElifBlock::castChildren(const ConversionTable&conversion_table) -> void {
        for (auto&[condition, body]: elifStatements) {
            condition->cast(conversion_table);
            body->cast(conversion_table);
        }
    }

    auto ElifBlock::optimize() -> core::SharedNode<> {
        for (auto&[condition, body]: elifStatements) {
            auto new_condition = condition->optimize();
            auto new_body = body->optimize();

            if (new_condition != nullptr) {
                condition = isl::staticPointerCast<AstlangNode>(std::move(new_condition));
            }

            if (new_body != nullptr) {
                body = isl::staticPointerCast<AstlangNode>(std::move(new_body));
            }
        }

        return nullptr;
    }
} // namespace astlang2::ast::statement
