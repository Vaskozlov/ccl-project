#ifndef CCL_PROJECT_LR_GSS_HPP
#define CCL_PROJECT_LR_GSS_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/nodes_lifetime_manager.hpp>
#include <ccl/parser/sppf_node.hpp>
#include <ccl/parser/table_entry.hpp>
#include <deque>

namespace ccl::parser::lr
{
    class GSS
    {
    public:
        struct Descriptor;

        struct Node
        {
            std::vector<Node *> previous;
            ast::Node *value{};
            SmallId inputPosition{};
            SmallId parserState{};

            auto addPrevious(Node *parent) -> void
            {
                auto it = std::ranges::find(previous, parent);

                if (it == previous.end()) {
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

        struct PushResult
        {
            Node *gssNode;
            bool isNew;
        };

        using Level = std::vector<std::unique_ptr<Node>>;
        using Levels = std::vector<Level>;

    private:
        struct Reducer
        {
            std::function<ast::NodeOfNodes *(std::vector<ast::Node *>)> reducer;
            const ankerl::unordered_dense::map<TableEntry, State> *gotoTable;
            GSS *gss;
            State production;
            SmallId inputLevel;

            auto reduce(SmallId pop_count, Node *node, std::vector<ast::Node *> arguments) -> void
            {
                pop_count -= 1;
                arguments.emplace_back(node->value);

                if (pop_count == 0) {
                    auto *new_node = reducer(std::move(arguments));
                    node->value->getLifetimeManager()->insert(new_node);

                    for (auto *prev : node->previous) {
                        auto new_state = gotoTable->at({
                            prev->parserState,
                            production,
                        });

                        auto [gss_node, inserted] =
                            gss->pushNonTerminal(prev, inputLevel, new_state, new_node);

                        auto new_descriptor = Descriptor{
                            .stack = gss_node,
                            .inputPosition = inputLevel,
                            .parserState = new_state,
                        };

                        gss->add(new_descriptor);
                    }

                    return;
                }

                for (auto *prev : node->previous) {
                    reduce(pop_count, prev, arguments);
                }
            }
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
            ast::TokenNode *token) -> PushResult
        {
            if (input_position >= levels.size()) {
                levels.resize(input_position + 1);
            }

            auto *node = find(input_position, parser_state);
            const auto inserted = (node == nullptr);

            if (node == nullptr) {
                auto &level = levels.at(input_position);
                auto created_node = std::make_unique<Node>();

                created_node->value = token;
                created_node->inputPosition = input_position;
                created_node->parserState = parser_state;

                node = created_node.get();
                level.emplace_back(std::move(created_node));
            }

            node->addPrevious(parent);

            return PushResult{
                .gssNode = node,
                .isNew = inserted,
            };
        }

        [[nodiscard]] auto pushNonTerminal(
            Node *parent, SmallId input_position, SmallId parser_state,
            ast::NodeOfNodes *non_terminal) -> PushResult
        {
            if (input_position >= levels.size()) {
                levels.resize(input_position + 1);
            }

            auto *node = find(input_position, parser_state);
            const auto inserted = (node == nullptr);

            if (node == nullptr || static_cast<ast::NodeOfNodes *>(node->value)->getNodes() !=
                                       non_terminal->getNodes()) {
                auto &level = levels.at(input_position);
                auto created_node = std::make_unique<Node>();

                created_node->value = non_terminal;

                created_node->inputPosition = input_position;
                created_node->parserState = parser_state;

                node = created_node.get();
                level.emplace_back(std::move(created_node));
            }

            node->addPrevious(parent);

            return PushResult{
                .gssNode = node,
                .isNew = inserted,
            };
        }

        auto add(Descriptor descriptor) -> void
        {
            if (passed.contains(descriptor)) {
                return;
            }

            passed.emplace(descriptor);

            // TODO: do not run same descriptor twice
            if (descriptor.inputPosition == globalInputPosition) {
                descriptors.emplace_back(descriptor);
            } else {
                descriptors.emplace_front(descriptor);
            }
        }

        auto getDescriptor() -> Descriptor
        {
            auto descriptor = descriptors.front();
            descriptors.pop_front();

            return descriptor;
        }

        auto reduce(
            SmallId pop_count, const ankerl::unordered_dense::map<TableEntry, State> *gotoTable,
            State production, std::function<ast::NodeOfNodes *(std::vector<ast::Node *>)> reducer,
            const Descriptor &descriptor) -> void
        {
            Reducer{
                .reducer = std::move(reducer),
                .gotoTable = gotoTable,
                .gss = this,
                .production = production,
                .inputLevel = descriptor.inputPosition,
            }
                .reduce(pop_count, descriptor.stack, {});
        }

    private:
        auto find(SmallId input_position, SmallId parser_state) -> Node *
        {
            const auto &level = levels.at(input_position);

            for (const auto &node : level) {
                if (node->parserState == parser_state) {
                    return node.get();
                }
            }

            return nullptr;
        }
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_HPP */
