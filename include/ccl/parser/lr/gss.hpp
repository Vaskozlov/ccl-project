#ifndef CCL_PROJECT_LR_GSS_HPP
#define CCL_PROJECT_LR_GSS_HPP

#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/table_entry.hpp>
#include <deque>

namespace ccl::parser::lr
{
    struct Node;

    struct Descriptor
    {
        Node *stack;
        SmallId inputPosition;
        SmallId parserState;

        auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser::lr

template<>
struct ankerl::unordered_dense::hash<ccl::parser::lr::Descriptor>
{
    using is_avalanching = void;

    [[nodiscard]] auto
        operator()(const ccl::parser::lr::Descriptor &descriptor) const noexcept -> auto
    {
        return detail::wyhash::hash(&descriptor, sizeof(ccl::parser::lr::Descriptor));
    }
};

namespace ccl::parser::lr
{
    struct Node
    {
        std::vector<Node *> previous;
        ast::SharedNode<> value{};
        SmallId inputPosition{};
        SmallId parserState{};

        auto addPrevious(Node *parent) -> void
        {
            for (auto *node : previous) {
                if (node == parent) {
                    return;
                }
            }

            previous.emplace_back(parent);
        }
    };
    namespace detail
    {
        // NOLINTNEXTLINE
        extern isl::FixedSizeAllocator<sizeof(Node), alignof(Node)> NodeAllocator;
    }// namespace detail

    using UniqueGssNodePtr = isl::UniquePtr<Node, std::addressof(detail::NodeAllocator)>;

    class GSS
    {
    public:
        struct Level
        {
            std::vector<UniqueGssNodePtr> terminals;
            std::vector<UniqueGssNodePtr> nonTerminals;

            [[nodiscard]] auto findTerminal(SmallId parser_state) const -> Node *;

            [[nodiscard]] auto
                findNonTerminal(SmallId parsing_state, ast::NodeOfNodes *value) const -> Node *;
        };

        using Levels = std::vector<Level>;

    private:
        struct Reducer
        {
            std::function<ast::SharedNode<ast::NodeOfNodes>(std::vector<ast::SharedNode<>>)>
                reducer;
            const ankerl::unordered_dense::map<TableEntry, State> *gotoTable;
            GSS *gss;
            State production;
            SmallId inputLevel;

            auto reduce(SmallId pop_count, Node *node, std::vector<ast::SharedNode<>> arguments)
                -> void;
        };

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
