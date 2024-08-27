#include <ccl/parser/lr/gss/gss.hpp>

namespace ccl::parser::lr
{
    // NOLINTNEXTLINE
    isl::FixedSizeAllocator<sizeof(Node), alignof(Node)> detail::NodeAllocator{};

    auto Reducer::reduce(SmallId pop_count, Node *node, std::vector<ast::SharedNode<>> arguments)
        -> void
    {
        pop_count -= 1;
        arguments.emplace_back(node->value);

        if (pop_count == 0) {
            auto new_node = reducer(std::move(arguments));

            for (auto *prev : node->previous) {
                auto new_state = gotoTable->at({
                    .state = prev->parserState,
                    .symbol = production,
                });

                auto *gss_node = gss->pushNonTerminal(prev, inputLevel, new_state, new_node);

                gss->add({
                    .stack = gss_node,
                    .inputPosition = inputLevel,
                    .parserState = new_state,
                });
            }

            return;
        }

        for (auto *prev : node->previous) {
            reduce(pop_count, prev, arguments);
        }
    }

    auto Level::findNonTerminal(ast::NonTerminal *value) const -> Node *
    {
        for (const auto &node : nonTerminals) {
            const auto *node_as_non_terminal =
                static_cast<const ast::NonTerminal *>(node->value.get());

            if (node_as_non_terminal->getNodes() == value->getNodes()) {
                return node.get();
            }
        }

        return nullptr;
    }

    auto GSS::pushTerminal(
        Node *parent,
        SmallId input_position,
        SmallId parser_state,
        ast::SharedNode<ast::Terminal>
            token) -> Node *
    {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        auto &level = levels.at(input_position);

        if (level.terminal == nullptr) {
            level.terminal = UniqueGssNodePtr{
                std::vector{parent},
                std::move(token),
                input_position,
                parser_state,
            };

            if (parent == nullptr) {
                level.terminal->previous.pop_back();
            }
        } else {
            level.terminal->addPrevious(parent);
        }

        return level.terminal.get();
    }

    auto GSS::pushNonTerminal(
        Node *parent,
        SmallId input_position,
        SmallId parser_state,
        ast::SharedNode<ast::NonTerminal>
            non_terminal) -> Node *
    {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        auto &level = levels.at(input_position);
        auto *node = level.findNonTerminal(non_terminal.get());

        // Is there any need to check?
        if (node == nullptr) {
            auto created_node = UniqueGssNodePtr{
                std::vector{parent},
                std::move(non_terminal),
                input_position,
                parser_state,
            };

            node = created_node.get();
            level.nonTerminals.emplace_back(std::move(created_node));
        } else {
            node->addPrevious(parent);
        }

        return node;
    }

    auto GSS::add(Descriptor descriptor) -> void
    {
        auto inserted = false;
        std::tie(std::ignore, inserted) =
            passed.at(descriptor.inputPosition % 2).emplace(descriptor);

        if (!inserted) {
            return;
        }

        if (descriptor.inputPosition == globalInputPosition) {
            descriptors.emplace_back(descriptor);
        } else {
            descriptors.emplace_front(descriptor);
        }
    }

    auto GSS::reduce(
        SmallId pop_count,
        const ankerl::unordered_dense::map<TableEntry, State> *gotoTable,
        State production,
        const std::function<ast::SharedNode<ast::NonTerminal>(std::vector<ast::SharedNode<>>)>
            &reducer,
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
}// namespace ccl::parser::lr