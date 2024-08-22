#ifndef CCL_PROJECT_LR_GSS_HPP
#define CCL_PROJECT_LR_GSS_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/table_entry.hpp>
#include <deque>

namespace ccl::parser::lr
{
    class GSS
    {
    public:
        struct Node
        {
            std::vector<Node *> previous;
            ast::Node *value{};
            SmallId inputPosition{};
            SmallId parserState{};

            auto addPrevious(Node *parent) -> void
            {
                if (auto it = std::ranges::find(previous, parent); it == previous.end()) {
                    previous.emplace_back(parent);
                }
            }
        };

        struct Descriptor
        {
            Node *stack;
            SmallId inputPosition;
            SmallId parserState;

            auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
        };

        struct Level
        {
            std::vector<std::unique_ptr<Node>> terminals;
            std::vector<std::unique_ptr<Node>> nonTerminals;

            [[nodiscard]] auto findTerminal(SmallId parser_state) const -> Node *;

            [[nodiscard]] auto
                findNonTerminal(SmallId parsing_state, ast::NodeOfNodes *value) const -> Node *;
        };

        using Levels = std::vector<Level>;

    private:
        struct Reducer
        {
            std::function<ast::NodeOfNodes *(std::vector<ast::Node *>)> reducer;
            const ankerl::unordered_dense::map<TableEntry, State> *gotoTable;
            GSS *gss;
            State production;
            SmallId inputLevel;

            auto reduce(SmallId pop_count, Node *node, std::vector<ast::Node *> arguments) -> void;
        };

        Levels levels;
        std::deque<Descriptor> descriptors;
        std::set<Descriptor> passed;
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
        }

        [[nodiscard]] auto pushTerminal(
            Node *parent, SmallId input_position, SmallId parser_state,
            ast::TokenNode *token) -> Node *;

        [[nodiscard]] auto pushNonTerminal(
            Node *parent, SmallId input_position, SmallId parser_state,
            ast::NodeOfNodes *non_terminal) -> Node *;

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
            const std::function<ast::NodeOfNodes *(std::vector<ast::Node *>)> &reducer,
            const Descriptor &descriptor) -> void;
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_HPP */
