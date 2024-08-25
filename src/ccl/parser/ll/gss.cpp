#include <ccl/parser/ll/gss.hpp>

namespace ccl::parser::ll
{
    isl::FixedSizeAllocator<sizeof(Node), alignof(Node)> detail::NodeAllocator;

    auto GSS::Level::findNode(const SPPFNode &sppf_node) const -> Node *
    {
        for (auto &node : *this) {
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

        for (Node *prev : descriptor.stack->previous) {
            auto new_sppf = prev->sppfNode;
            new_sppf.next(constructed_node);

            auto *new_node = createNode(prev->previous, std::move(new_sppf), input_position);

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

        std::tie(std::ignore, inserted) = current_set.emplace(
            PassedDescriptor{
                .sppfNode = node->sppfNode,
                .inputPosition = node->inputPosition,
            });

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
        if (nonTerminalDescriptors.empty()) {
            auto descriptor = terminalDescriptors.front();
            terminalDescriptors.pop_front();
            return descriptor;
        }

        if (terminalDescriptors.empty()) {
            auto descriptor = nonTerminalDescriptors.front();
            nonTerminalDescriptors.pop_front();

            return descriptor;
        }

        if (nonTerminalDescriptors.front().inputPosition <=
            terminalDescriptors.front().inputPosition) {
            auto descriptor = nonTerminalDescriptors.front();
            nonTerminalDescriptors.pop_front();

            return descriptor;
        }

        auto descriptor = terminalDescriptors.front();
        terminalDescriptors.pop_front();

        return descriptor;
    }

    auto GSS::createNode(Node *parent, SPPFNode sppf_node, SmallId input_position) -> Node *
    {
        auto &level = getLevel(input_position);
        auto *node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto constructed_node =
                UniqueGssNodePtr(std::vector{parent}, std::move(sppf_node), input_position);

            node = constructed_node.get();
            level.emplace_back(std::move(constructed_node));

            return node;
        }

        node->addParent(parent);
        return node;
    }

    auto GSS::createNode(
        const std::vector<Node *> &parents,
        SPPFNode sppf_node,
        SmallId input_position) -> Node *
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


    auto GSS::getLevel(SmallId input_position) -> Level &
    {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        return levels.at(input_position);
    }
}// namespace ccl::parser::ll
