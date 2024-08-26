#ifndef CCL_PROJECT_LR_GSS_HPP
#define CCL_PROJECT_LR_GSS_HPP

#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/lr/gss/descriptor.hpp>
#include <ccl/parser/lr/gss/level.hpp>
#include <ccl/parser/lr/gss/node.hpp>
#include <ccl/parser/lr/gss/reducer.hpp>
#include <ccl/parser/table_entry.hpp>
#include <deque>

namespace ccl::parser::lr
{
    class GSS
    {
    public:
        using Levels = std::vector<Level>;

    private:
        Levels levels;
        std::deque<Descriptor> descriptors;
        std::array<ankerl::unordered_dense::set<Descriptor>, 2> passed;
        SmallId globalInputPosition{};

    public:
        [[nodiscard]] auto getGlobalInputPosition() const noexcept -> SmallId
        {
            return globalInputPosition;
        }

        [[nodiscard]] auto getLevels() const noexcept -> const Levels &
        {
            return levels;
        }

        [[nodiscard]] auto hasDescriptors() const noexcept -> bool
        {
            return !descriptors.empty();
        }

        auto nextWord() -> void
        {
            ++globalInputPosition;
            passed.at(globalInputPosition % 2).clear();
        }

        [[nodiscard]] auto pushTerminal(
            Node *parent, SmallId input_position, SmallId parser_state,
            ast::SharedNode<ast::TokenNode> token) -> Node *;

        [[nodiscard]] auto pushNonTerminal(
            Node *parent, SmallId input_position, SmallId parser_state,
            ast::SharedNode<ast::NodeOfNodes> non_terminal) -> Node *;

        auto add(Descriptor descriptor) -> void;

        auto getDescriptor() -> Descriptor
        {
            auto descriptor = descriptors.front();
            descriptors.pop_front();

            return descriptor;
        }

        auto reduce(
            SmallId pop_count, const ankerl::unordered_dense::map<TableEntry, State> *gotoTable,
            State production,
            const std::function<ast::SharedNode<ast::NodeOfNodes>(std::vector<ast::SharedNode<>>)>
                &reducer,
            const Descriptor &descriptor) -> void;
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_HPP */
