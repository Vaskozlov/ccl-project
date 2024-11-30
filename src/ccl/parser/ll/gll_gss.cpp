#include <ccl/parser/ll/gss/gss.hpp>

namespace ccl::parser::ll
{
    isl::PoolAllocator<sizeof(Node), alignof(Node)> detail::NodeAllocator;

    auto Level::findNode(const SPPFNode &sppf_node) const -> Node *
    {
        for (const auto &node : *this) {
            if (node->sppfNode == sppf_node) {
                return node.get();
            }
        }

        return nullptr;
    }

    auto GSS::pop(const Descriptor &descriptor) -> void
    {
        const auto input_position = descriptor.inputPosition;
        auto constructed_node = descriptor.stack->sppfNode.build();

        for (const Node *prev : descriptor.stack->previous) {
            auto new_sppf = prev->sppfNode;
            new_sppf.next(constructed_node);

            auto *new_node = pushNode(prev->previous, std::move(new_sppf), input_position);

            add({
                .stack = new_node,
                .inputPosition = input_position,
            });
        }
    }

    auto GSS::add(Descriptor descriptor) -> void
    {
        auto inserted = false;
        const auto *node = descriptor.stack;

        const auto input_position = descriptor.inputPosition;
        auto &current_set = passed.at(input_position % 2);

        std::tie(std::ignore, inserted) = current_set.emplace(node);

        if (!inserted) {
            return;
        }

        const auto at_dot =
            node->sppfNode.rule.isDotInTheEnd() ? 0 : descriptor.stack->sppfNode.atDot();

        const auto is_current_symbol_a_terminal = storage->isTerminal(at_dot);

        if (descriptor.inputPosition == globalInputPosition) {
            if (is_current_symbol_a_terminal) {
                terminalDescriptors.emplace_back(descriptor);
                return;
            }

            nonTerminalDescriptors.emplace_back(descriptor);
        }

        if (is_current_symbol_a_terminal) {
            terminalDescriptors.emplace_front(descriptor);
            return;
        }

        nonTerminalDescriptors.emplace_front(descriptor);
    }

    auto GSS::getDescriptor() -> Descriptor
    {
        const auto non_terminals_empty = nonTerminalDescriptors.empty();
        const auto terminals_empty = terminalDescriptors.empty();

        if (non_terminals_empty || terminals_empty) {
            auto &selectedList = non_terminals_empty ? terminalDescriptors : nonTerminalDescriptors;

            const auto descriptor = selectedList.front();
            selectedList.pop_front();

            return descriptor;
        }

        const auto &non_terminal_front = nonTerminalDescriptors.front();
        const auto &[stack, inputPosition] = terminalDescriptors.front();

        auto &selectedList = (non_terminal_front.inputPosition <= inputPosition)
                                 ? nonTerminalDescriptors
                                 : terminalDescriptors;

        auto descriptor = selectedList.front();
        selectedList.pop_front();

        return descriptor;
    }

    auto GSS::pushNode(Node *parent, SPPFNode sppf_node, SmallId input_position) -> Node *
    {
        auto &level = getLevel(input_position);
        auto *node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto constructed_node = UniqueGssNodePtr{
                Node::Vector{parent},
                std::move(sppf_node),
                input_position,
            };

            node = constructed_node.get();
            level.emplace_back(std::move(constructed_node));

            return node;
        }

        node->addParent(parent);
        return node;
    }

    auto GSS::pushNode(const Node::Vector &parents, SPPFNode sppf_node, SmallId input_position)
        -> Node *
    {
        auto &level = getLevel(input_position);
        auto *node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto constructed_node = UniqueGssNodePtr(parents, std::move(sppf_node), input_position);

            node = constructed_node.get();
            level.emplace_back(std::move(constructed_node));

            return node;
        }

        for (auto *parent : parents) {
            node->addParent(parent);
        }

        return node;
    }


    auto GSS::getLevel(const SmallId input_position) -> Level &
    {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
            levels.back().reserve(8);
        }

        return levels.at(input_position);
    }
}// namespace ccl::parser::ll
